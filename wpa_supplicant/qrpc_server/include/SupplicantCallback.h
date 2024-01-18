/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "aidl/android/hardware/wifi/supplicant/ISupplicantCallback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class SupplicantCallback : public ISupplicantCallback {
public:
	SupplicantCallback() {}
	~SupplicantCallback() {}

	::ndk::ScopedAStatus onInterfaceCreated(const std::string& in_ifaceName) override;
	::ndk::ScopedAStatus onInterfaceRemoved(const std::string& in_ifaceName) override;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
