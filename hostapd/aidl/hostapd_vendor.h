/*
 * aidl interface for wpa_hostapd daemon
 * Copyright (c) 2004-2018, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2018, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <map>
#include <string>

#include <android-base/macros.h>

#include <aidl/vendor/qti/hardware/wifi/hostapd/BnHostapdVendor.h>
#include <aidl/vendor/qti/hardware/wifi/hostapd/IHostapdVendor.h>
#include <aidl/vendor/qti/hardware/wifi/hostapd/IHostapdVendorCallback.h>

extern "C"
{
#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/includes.h"
#include "utils/wpa_debug.h"
#include "ap/hostapd.h"
#include "ap/sta_info.h"
}

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace hostapd {

/**
 * Implementation of the hostapd aidl object. This aidl
 * object is used core for global control operations on
 * hostapd.
 */
class HostapdVendor : public BnHostapdVendor
{
public:
	HostapdVendor(hapd_interfaces* interfaces);
	~HostapdVendor() override = default;
	bool isValid();

	// Aidl methods exposed.
	::ndk::ScopedAStatus listVendorInterfaces(
		std::vector<std::string>* _aidl_return) override;
	::ndk::ScopedAStatus registerHostapdVendorCallback(
	        const std::shared_ptr<IHostapdVendorCallback>& callback) override;
	::ndk::ScopedAStatus doDriverCmd(
		const std::string& iface, const std::string& cmd, std::string* _aidl_return) override;
private:
	// Corresponding worker functions for the AIDL methods.
	::ndk::ScopedAStatus registerHostapdVendorCallbackInternal(
		const std::shared_ptr<IHostapdVendorCallback>& callback);
	std::pair<std::vector<std::string>, ndk::ScopedAStatus> listVendorInterfacesInternal();
	std::pair<std::string, ndk::ScopedAStatus> doDriverCmdInternal(const std::string& iface, const std::string& cmd);

	// Raw pointer to the global structure maintained by the core.
	struct hapd_interfaces* interfaces_;
	// Callbacks registered.
	std::vector<std::shared_ptr<IHostapdVendorCallback>> callbacks_;
	// Death notifier.
	AIBinder_DeathRecipient* death_notifier_;
	DISALLOW_COPY_AND_ASSIGN(HostapdVendor);
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
