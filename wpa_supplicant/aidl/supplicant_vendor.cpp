/*
 * WPA Supplicant - Supplicant Aidl interface
 * Copyright (c) 2021, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "aidl_manager.h"
#include "aidl_return_util.h"
#include "misc_utils.h"
#include "supplicant.h"
#include "supplicant_vendor.h"
#include "p2p_iface.h"

#include <android-base/file.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
using android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;
using android::hardware::wifi::supplicant::misc_utils::createStatus;
using android::hardware::wifi::supplicant::misc_utils::createStatusWithMsg;
using android::hardware::wifi::supplicant::SupplicantStatusCode;
using android::hardware::wifi::supplicant::AidlManager;
using android::hardware::wifi::supplicant::IfaceInfo;
using android::hardware::wifi::supplicant::IfaceType;

SupplicantVendor::SupplicantVendor(struct wpa_global* global) : wpa_global_(global) {}
bool SupplicantVendor::isValid()
{
	// This top level object cannot be invalidated.
	return true;
}

::ndk::ScopedAStatus SupplicantVendor::listVendorInterfaces(
	std::vector<IVendorIfaceInfo>* _aidl_return)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&SupplicantVendor::listVendorInterfacesInternal, _aidl_return);
}

::ndk::ScopedAStatus SupplicantVendor::getVendorInterface(
	const IVendorIfaceInfo& info, std::shared_ptr<ISupplicantVendorStaIface>* _aidl_return)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&SupplicantVendor::getVendorInterfaceInternal, _aidl_return, info);
}

std::pair<std::vector<IVendorIfaceInfo>, ndk::ScopedAStatus>
SupplicantVendor::listVendorInterfacesInternal()
{
	std::vector<IVendorIfaceInfo> ifaces;
	for (struct wpa_supplicant* wpa_s = wpa_global_->ifaces; wpa_s;
		 wpa_s = wpa_s->next) {
		if (wpa_s->global->p2p_init_wpa_s == wpa_s) {

		} else {
			ifaces.emplace_back(IVendorIfaceInfo{IVendorIfaceType::STA, wpa_s->ifname});
		}
	}
	return {std::move(ifaces), ndk::ScopedAStatus::ok()};
}

std::pair<std::shared_ptr<ISupplicantVendorStaIface>, ndk::ScopedAStatus>
SupplicantVendor::getVendorInterfaceInternal(const IVendorIfaceInfo& info)
{
	struct wpa_supplicant* wpa_s =
	    wpa_supplicant_get_iface(wpa_global_, info.name.c_str());
	if (!wpa_s) {
		return {nullptr, createStatus(SupplicantStatusCode::FAILURE_IFACE_UNKNOWN)};
	}
	AidlManager* aidl_manager = AidlManager::getInstance();
	std::shared_ptr<ISupplicantVendorStaIface> iface;
	if (!aidl_manager ||
	    aidl_manager->getVendorStaIfaceAidlObjectByIfname(
	    wpa_s->ifname, &iface)) {
		return {iface, createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}
	return {iface, ndk::ScopedAStatus::ok()};
}

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
