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

#ifndef WPA_SUPPLICANT_VENDOR_AIDL_STA_IFACE_H
#define WPA_SUPPLICANT_VENDOR_AIDL_STA_IFACE_H

#include <array>
#include <vector>

#include <android-base/macros.h>

#include <aidl/vendor/qti/hardware/wifi/supplicant/BnSupplicantVendorStaIface.h>

#include "SupplicantVendorStaIfaceCallback.h"

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "../wpa_supplicant/wpa_supplicant_i.h"
#include "../wpa_supplicant/config.h"
#include "../wpa_supplicant/driver_i.h"
#include "rsn_supp/wpa.h"
}

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {

/**
 * Implementation of StaIface aidl object. Each unique aidl
 * object is used for control operations on a specific interface
 * controlled by wpa_supplicant.
 */
class VendorStaIface : public BnSupplicantVendorStaIface
{
public:
	VendorStaIface(struct wpa_global* wpa_global, const char ifname[], int32_t id);
	~VendorStaIface() = default;
	// AIDL does not provide a built-in mechanism to let the server
	// invalidate a AIDL interface object after creation. If any client
	// process holds onto a reference to the object in their context,
	// any method calls on that reference will continue to be directed to
	// the server.
	// However Supplicant HAL needs to control the lifetime of these
	// objects. So, add a public |invalidate| method to all |Iface| and
	// |Network| objects.
	// This will be used to mark an object invalid when the corresponding
	// iface or network is removed.
	// All AIDL method implementations should check if the object is still
	// marked valid before processing them.
	void invalidate();
	bool isValid();

	// Aidl methods exposed.
	::ndk::ScopedAStatus registerSupplicantVendorStaIfaceCallback(
		const std::shared_ptr<SupplicantVendorStaIfaceCallback>& in_callback);
	::ndk::ScopedAStatus doDriverCmd(
		const std::string& cmd, std::string* _aidl_return) override;

private:
	// Corresponding worker functions for the AIDL methods.
	ndk::ScopedAStatus registerSupplicantVendorStaIfaceCallbackInternal(
		const std::shared_ptr<SupplicantVendorStaIfaceCallback>& callback);
	std::pair<std::string, ndk::ScopedAStatus> doDriverCmdInternal(
		const std::string& cmd);

	struct wpa_supplicant* retrieveIfacePtr();

	// Reference to the global wpa_struct. This is assumed to be valid for
	// the lifetime of the process.
	struct wpa_global* wpa_global_;
	// Name of the iface this aidl object controls
	const std::string ifname_;
	// Unique id for someip instance
	const int32_t ifId_;
	bool is_valid_;

	DISALLOW_COPY_AND_ASSIGN(VendorStaIface);
};

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl

#endif  // WPA_SUPPLICANT_VENDOR_AIDL_STA_IFACE_H
