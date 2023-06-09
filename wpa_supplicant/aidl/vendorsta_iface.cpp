/*
 * WPA Supplicant - Sta Iface Aidl interface
 * Copyright (c) 2021, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
/*
 * Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "aidl_manager.h"
#include "aidl_return_util.h"
#include "iface_config_utils.h"
#include "misc_utils.h"
#include "sta_iface.h"
#include "vendorsta_iface.h"

extern "C"
{
#include "utils/eloop.h"
#include "gas_query.h"
#include "interworking.h"
#include "hs20_supplicant.h"
#include "wps_supplicant.h"
#include "dpp.h"
#include "dpp_supplicant.h"
#include "rsn_supp/wpa.h"
#include "rsn_supp/pmksa_cache.h"
}

namespace {
using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::vendor::qti::hardware::wifi::supplicant::ISupplicantVendorStaIface;
using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;
using aidl::android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;

}  // namespace

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {

using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;

VendorStaIface::VendorStaIface(struct wpa_global *wpa_global, const char ifname[])
	: wpa_global_(wpa_global), ifname_(ifname), is_valid_(true)
{}

void VendorStaIface::invalidate() { is_valid_ = false; }
bool VendorStaIface::isValid()
{
	return (is_valid_ && (retrieveIfacePtr() != nullptr));
}

::ndk::ScopedAStatus VendorStaIface::registerSupplicantVendorStaIfaceCallback(
	const std::shared_ptr<ISupplicantVendorStaIfaceCallback>& in_callback)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&VendorStaIface::registerSupplicantVendorStaIfaceCallbackInternal, in_callback);
}

::ndk::ScopedAStatus VendorStaIface::doDriverCmd(
	const std::string& cmd, std::string* _aidl_return)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_UNKNOWN,
		&VendorStaIface::doDriverCmdInternal, _aidl_return, cmd);
}

ndk::ScopedAStatus VendorStaIface::registerSupplicantVendorStaIfaceCallbackInternal(
	const std::shared_ptr<ISupplicantVendorStaIfaceCallback> &callback)
{
	AidlManager *aidl_manager = AidlManager::getInstance();
	if (!aidl_manager ||
		aidl_manager->addVendorStaIfaceCallbackAidlObject(ifname_, callback)) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	return ndk::ScopedAStatus::ok();
}

std::pair<std::string, ndk::ScopedAStatus>
VendorStaIface::doDriverCmdInternal(const std::string& cmd)
{
	struct wpa_supplicant *wpa_s = retrieveIfacePtr();
	if (!wpa_s) {
		return {"", createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}
	wpa_printf(MSG_INFO, "doDriverCmd[%s] - '%s'\n",
		ifname_.c_str(), cmd.c_str());

	char *reply;
	const int reply_size = 4096;
	int ret;
	reply = (char*) os_zalloc(reply_size);
	if (reply == NULL) {
		return {"", createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}
	/*
	 * ret > 0: GET success, buffer already filled.
	 * ret = 0: SET success, fill with buffer 'OK'.
	 * ret < 0: SET/GET fail, fill with buffer 'FAIL'.
	 */
	ret = wpa_drv_driver_cmd(wpa_s, (char *)cmd.c_str(), reply, reply_size);
	if (ret == 0) {
		ret = os_snprintf(reply, reply_size, "%s\n", "OK");
		if (os_snprintf_error(reply_size, ret))
			ret = -1;
	}
	if (ret < 0) {
		os_memcpy(reply, "FAIL\n", 5);
	}
	std::string str_reply(reply);
	os_free(reply);

	return {std::move(str_reply), ndk::ScopedAStatus::ok()};
}

/**
 * Retrieve the underlying |wpa_supplicant| struct
 * pointer for this iface.
 * If the underlying iface is removed, then all RPC method calls on this object
 * will return failure.
 */
wpa_supplicant *VendorStaIface::retrieveIfacePtr()
{
	return wpa_supplicant_get_iface(wpa_global_, ifname_.c_str());
}

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
