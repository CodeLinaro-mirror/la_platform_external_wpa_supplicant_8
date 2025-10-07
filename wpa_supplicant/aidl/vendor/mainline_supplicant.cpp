/*
 * WPA Supplicant - Root mainline supplicant interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "aidl/shared/shared_utils.h"
#include "aidl_manager.h"
#include "aidl_return_util.h"
#include "driver_i.h"
#include "mainline_supplicant.h"
#include "misc_utils.h"
#include "nan_iface.h"

using aidl::android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;
using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;
using aidl::android::system::wifi::mainline_supplicant::ISupplicantNanIface;

const std::string kConfigFilePath = "/apex/com.android.wifi/etc/wpa_supplicant_mainline.conf";

MainlineSupplicant::MainlineSupplicant(struct wpa_global* global)
	: wpa_global_(global)
{
	wpa_printf(MSG_INFO, "Creating the mainline supplicant instance");
	vendor_supplicant_ = ndk::SharedRefBase::make<Supplicant>(global);
}

bool MainlineSupplicant::isValid() {
	// This top level object cannot be invalidated.
	return true;
}

::ndk::ScopedAStatus MainlineSupplicant::getVendorSupplicant(
	std::shared_ptr<ISupplicant>* _aidl_return)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&MainlineSupplicant::getVendorSupplicantInternal, _aidl_return);
}

::ndk::ScopedAStatus MainlineSupplicant::addNanInterface(
	const std::string& in_ifaceName,
	std::shared_ptr<ISupplicantNanIface>* _aidl_return)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&MainlineSupplicant::addNanInterfaceInternal, _aidl_return,
		in_ifaceName);
}

::ndk::ScopedAStatus MainlineSupplicant::removeNanInterface(
        const std::string& in_ifaceName) {
    wpa_printf(MSG_ERROR, "removeNanInterface is not supported");
    return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

std::pair<std::shared_ptr<ISupplicant>, ndk::ScopedAStatus>
MainlineSupplicant::getVendorSupplicantInternal()
{
	return {vendor_supplicant_, ndk::ScopedAStatus::ok()};
}

std::pair<std::shared_ptr<ISupplicantNanIface>, ndk::ScopedAStatus>
MainlineSupplicant::addNanInterfaceInternal(const std::string& ifaceName)
{
	u8 addr[ETH_ALEN];
	size_t pos = ifaceName.find('-');
	if (ifaceName.empty() || pos == std::string::npos) {
		wpa_printf(
			MSG_ERROR, "Invalid iface name format: %s",
			ifaceName.c_str());
		return {
			nullptr,
			createStatus(SupplicantStatusCode::FAILURE_ARGS_INVALID)};
	}

	std::string nan_iface_name, primary_iface_name;
	nan_iface_name = ifaceName.substr(0, pos);
	primary_iface_name = ifaceName.substr(pos + 1);

	if (active_nan_ifaces_.find(nan_iface_name) != active_nan_ifaces_.end()) {
		wpa_printf(MSG_ERROR, "NAN interface already exists");
		std::shared_ptr<ISupplicantNanIface> nan_iface =
			active_nan_ifaces_[nan_iface_name];
		return {nan_iface, ndk::ScopedAStatus::ok()};
	}
	struct wpa_supplicant* wpa_s =
		wpa_supplicant_get_iface(wpa_global_, primary_iface_name.c_str());

	if (!wpa_s) {
		wpa_printf(MSG_ERROR, "nl80211 parent interface not found");
		return {
			nullptr,
			createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}

	if (ensureConfigFileExistsAtPath(kConfigFilePath) != 0) {
		wpa_printf(
			MSG_ERROR, "Conf file does not exist: %s",
			kConfigFilePath.c_str());
		return {
			nullptr,
			createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}

	if (wpa_drv_if_add(
		wpa_s, WPA_IF_NAN, nan_iface_name.c_str(), NULL, NULL, NULL, addr,
		NULL) < 0) {
		wpa_printf(MSG_ERROR, "Failed to create NAN iface");
		return {
			nullptr,
			createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}

	struct wpa_interface iface_params = {};
	iface_params.driver = kIfaceDriverName;
	iface_params.ifname = nan_iface_name.c_str();
	iface_params.confname = kConfigFilePath.c_str();

	struct wpa_supplicant* nan_wpa_s =
		wpa_supplicant_add_iface(wpa_global_, &iface_params, NULL);
	if (!nan_wpa_s) {
		wpa_drv_if_remove(wpa_s, WPA_IF_NAN, nan_iface_name.c_str());
		wpa_printf(
			MSG_ERROR, "Unable to add interface %s",
			nan_iface_name.c_str());
		return {
			nullptr,
			createStatus(SupplicantStatusCode::FAILURE_UNKNOWN)};
	}

	std::shared_ptr<ISupplicantNanIface> nanIface =
		ndk::SharedRefBase::make<NanIface>(wpa_global_, nan_iface_name);
	active_nan_ifaces_[nan_iface_name] = nanIface;

	wpa_printf(
		MSG_INFO, "Interface %s added successfully", nan_iface_name.c_str());
	return {nanIface, ndk::ScopedAStatus::ok()};
}
