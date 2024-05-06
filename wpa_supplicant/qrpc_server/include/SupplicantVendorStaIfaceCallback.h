/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "aidl/vendor/qti/hardware/wifi/supplicant/ISupplicantVendorStaIfaceCallback.h"

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
class SupplicantVendorStaIfaceCallback : public ISupplicantVendorStaIfaceCallback {
public:
	SupplicantVendorStaIfaceCallback(int32_t iface_instance_id) : iface_instance_id_(iface_instance_id) {}
	~SupplicantVendorStaIfaceCallback() {}

	int32_t getIfaceInstanceId();

	::ndk::ScopedAStatus onCtrlEvent(const std::string& in_ifaceName, const std::string& in_eventStr) override;

private:
	const int32_t iface_instance_id_;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
