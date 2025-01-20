/*
 * aidl interface for wpa_hostapd daemon
 * Copyright (c) 2004-2018, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2018, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
/*
 * Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_bridge.h>

#include <android-base/file.h>
#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>

#include "hostapd.h"
#include "hostapd_vendor.h"
#include "aidl_return_util.h"
#include <aidl/android/hardware/wifi/hostapd/ApInfo.h>
#include <aidl/android/hardware/wifi/hostapd/BandMask.h>
#include <aidl/android/hardware/wifi/hostapd/ChannelParams.h>
#include <aidl/android/hardware/wifi/hostapd/ClientInfo.h>
#include <aidl/android/hardware/wifi/hostapd/EncryptionType.h>
#include <aidl/android/hardware/wifi/hostapd/HostapdStatusCode.h>
#include <aidl/android/hardware/wifi/hostapd/IfaceParams.h>
#include <aidl/android/hardware/wifi/hostapd/NetworkParams.h>
#include <aidl/android/hardware/wifi/hostapd/ParamSizeLimits.h>
#include <aidl/vendor/qti/hardware/wifi/hostapd/IHostapdVendorCallback.h>

extern "C"
{
#include "common/wpa_ctrl.h"
#include "drivers/linux_ioctl.h"
#include "ctrl_iface.h"
}

// The AIDL implementation for hostapd creates a hostapd.conf dynamically for
// each interface. This file can then be used to hook onto the normal config
// file parsing logic in hostapd code.  Helps us to avoid duplication of code
// in the AIDL interface.
// TOOD(b/71872409): Add unit tests for this.
namespace {

using aidl::android::hardware::wifi::hostapd::HostapdStatusCode;

inline ndk::ScopedAStatus createStatus(HostapdStatusCode status_code) {
	return ndk::ScopedAStatus::fromServiceSpecificError(
		static_cast<int32_t>(status_code));
}

// Method called by death_notifier_ on client death.
void onDeath(void* cookie) {
	wpa_printf(MSG_ERROR, "Client died. Terminating...");
	eloop_terminate();
}

std::function<void(struct hostapd_data*, const char*)> on_ctrl_event_internal_callback;
void onVendorCtrlEventCb(void *ctx, const char* msg)
{
	struct hostapd_data* iface_hapd = (struct hostapd_data*)ctx;
	if (!iface_hapd || !msg)
		return;
	if (on_ctrl_event_internal_callback) {
		 on_ctrl_event_internal_callback(iface_hapd, msg);
	}
}

}  // namespace

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace hostapd {

using android::hardware::wifi::hostapd::aidl_return_util::validateAndCall;
using android::hardware::wifi::hostapd::HostapdStatusCode;

HostapdVendor::HostapdVendor(struct hapd_interfaces* interfaces)
	: interfaces_(interfaces)
{
	death_notifier_ = AIBinder_DeathRecipient_new(onDeath);
}

bool HostapdVendor::isValid()
{
	return true;
}

::ndk::ScopedAStatus HostapdVendor::registerHostapdVendorCallback(
	const std::shared_ptr<IHostapdVendorCallback>& callback)
{
	return registerHostapdVendorCallbackInternal(callback);
}

::ndk::ScopedAStatus HostapdVendor::listVendorInterfaces(
	std::vector<std::string>* _aidl_return)
{
	return validateAndCall(
		this, HostapdStatusCode::FAILURE_UNKNOWN,
		&HostapdVendor::listVendorInterfacesInternal, _aidl_return);
}

::ndk::ScopedAStatus HostapdVendor::doDriverCmd(
	const std::string& iface, const std::string& cmd, std::string* _aidl_return)
{
	return validateAndCall(
		this, HostapdStatusCode::FAILURE_UNKNOWN,
		&HostapdVendor::doDriverCmdInternal, _aidl_return, iface, cmd);
}

::ndk::ScopedAStatus HostapdVendor::registerHostapdVendorCallbackInternal(
	const std::shared_ptr<IHostapdVendorCallback>& callback)
{
	if (!callback) {
		wpa_printf(MSG_ERROR, "callback null");
		return createStatus(HostapdStatusCode::FAILURE_UNKNOWN);
	}
	binder_status_t status = AIBinder_linkToDeath(callback->asBinder().get(),
			death_notifier_, this /* cookie */);
	if (status != STATUS_OK) {
		wpa_printf(
			MSG_ERROR,
			"Error registering for death notification for "
			"hostapd callback object");
		return createStatus(HostapdStatusCode::FAILURE_UNKNOWN);
	}
	// no iface_name provided, treat it as global callback
	// Hook the hapd callback if not registered.
	int i, j;
	if (!on_ctrl_event_internal_callback) {
		on_ctrl_event_internal_callback =
			[this](struct hostapd_data* iface_hapd, const char *msg) {
			if (callbacks_.size() == 0) {
				wpa_printf(MSG_ERROR, "No hostapd vendor callback registered");
				return;
			}
			const std::string ifname(iface_hapd->conf->iface);
			const std::string event_str(msg);
			for (const auto& callback : callbacks_) {
				callback->onCtrlEvent(ifname, event_str);
			}
		};
	}
	for (i = 0; i < interfaces_->count; i++) {
		struct hostapd_iface *iface = interfaces_->iface[i];
		for (j = 0; j < iface->num_bss; j++) {
			struct hostapd_data *hapd = iface->bss[j];
			if (!hapd->ctrl_event_aidl_cb) {
				hapd->ctrl_event_aidl_cb = onVendorCtrlEventCb;
				hapd->ctrl_event_aidl_cb_ctx = hapd;
			}
		}
	}
	callbacks_.push_back(callback);
	return ndk::ScopedAStatus::ok();
}

std::pair<std::vector<std::string>, ndk::ScopedAStatus>
HostapdVendor::listVendorInterfacesInternal()
{
	std::vector<std::string> ifaces;
	if (!interfaces_ || !interfaces_->iface) {
		return {ifaces, createStatus(HostapdStatusCode::FAILURE_UNKNOWN)};
	}
	for (size_t i = 0; i < interfaces_->count; i++) {
		struct hostapd_iface *iface = interfaces_->iface[i];
		for (size_t j = 0; j < iface->num_bss; j++) {
			struct hostapd_data *hapd = iface->bss[j];
			ifaces.emplace_back(hapd->conf->iface);
		}
	}
	wpa_printf(MSG_INFO, "listVendorInterfaces() - size=%d", (int)ifaces.size());
	return {std::move(ifaces), ndk::ScopedAStatus::ok()};
}

std::pair<std::string, ndk::ScopedAStatus>
HostapdVendor::doDriverCmdInternal(const std::string& iface_name, const std::string& cmd)
{
	if (!interfaces_ || !interfaces_->iface) {
		return {"", createStatus(HostapdStatusCode::FAILURE_UNKNOWN)};
	}
	wpa_printf(MSG_INFO, "doDriverCmd[%s] - '%s'\n",
	iface_name.c_str() , cmd.c_str());
	struct hostapd_data* iface_hapd =
	hostapd_get_iface(interfaces_, iface_name.c_str());
	if (!iface_hapd || !iface_hapd->iface) {
		return {"", createStatus(HostapdStatusCode::FAILURE_UNKNOWN)};
	}
	if (iface_hapd->iface->state != HAPD_IFACE_ENABLED
	    && iface_hapd->iface->state != HAPD_IFACE_DFS) {
		return {"", createStatus(HostapdStatusCode::FAILURE_UNKNOWN)};
	}
	char *reply;
	const int reply_size = 4096;
	int reply_len;
	reply = (char*) os_malloc(reply_size);
	if (reply == NULL) {
		return {"", createStatus(HostapdStatusCode::FAILURE_UNKNOWN)};
	}
	reply_len = hostapd_ctrl_iface_receive_process(iface_hapd, (char *)cmd.c_str(),
		reply, reply_size,
		NULL, 0);
	if (reply_len > reply_size) reply_len = reply_size;
	reply[reply_len] = '\0'; // make sure '\0' terminated.
	wpa_printf(MSG_INFO, "reply = %s", reply);
	std::string str_reply(reply);
	os_free(reply);
	return {std::move(str_reply), ndk::ScopedAStatus::ok()};
}

}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
