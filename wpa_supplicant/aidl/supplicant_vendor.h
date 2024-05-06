/*
 * WPA Supplicant - Supplicant Aidl interface
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

#ifndef WPA_SUPPLICANT_VENDOR_AIDL_SUPPLICANT_H
#define WPA_SUPPLICANT_VENDOR_AIDL_SUPPLICANT_H

#include <aidl/vendor/qti/hardware/wifi/supplicant/BnSupplicantVendor.h>

#include <android-base/macros.h>

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "utils/wpa_debug.h"
#include "../wpa_supplicant/wpa_supplicant_i.h"
#include "../wpa_supplicant/scan.h"
}

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {

/**
 * Implementation of the supplicant aidl object. This aidl
 * object is used core for global control operations on
 * wpa_supplicant.
 */
class SupplicantVendor : public BnSupplicantVendor
{
public:
	SupplicantVendor(struct wpa_global* global);
	~SupplicantVendor() = default;
	bool isValid();

	// Aidl methods exposed.
	::ndk::ScopedAStatus listVendorInterfaces(
		std::vector<IVendorIfaceInfo>* _aidl_return) override;
	::ndk::ScopedAStatus getVendorInterface(
		const IVendorIfaceInfo& info, std::shared_ptr<ISupplicantVendorStaIface>* _aidl_return) override;

private:
	// Corresponding worker functions for the AIDL methods.
	std::pair<std::vector<IVendorIfaceInfo>, ndk::ScopedAStatus> listVendorInterfacesInternal();
	std::pair<std::shared_ptr<ISupplicantVendorStaIface>, ndk::ScopedAStatus> getVendorInterfaceInternal(const IVendorIfaceInfo& info);

	// Raw pointer to the global structure maintained by the core.
	struct wpa_global* wpa_global_;
	// Driver name to be used for creating interfaces.
	static const char kDriverName[];
	// wpa_supplicant.conf file location on the device.
	static const char kConfigFilePath[];

	DISALLOW_COPY_AND_ASSIGN(SupplicantVendor);
};

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl

#endif  // WPA_SUPPLICANT_VENDOR_AIDL_SUPPLICANT_H
