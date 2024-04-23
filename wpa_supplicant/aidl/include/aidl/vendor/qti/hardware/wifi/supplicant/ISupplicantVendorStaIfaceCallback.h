/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantVendorStaIfaceCallback : public ::ndk::ICInterface {
public:
//  ISupplicantVendorStaIfaceCallback();
//  virtual ~ISupplicantVendorStaIfaceCallback();

  static const int32_t version = 2;

  virtual ::ndk::ScopedAStatus onCtrlEvent(const std::string& in_ifaceName, const std::string& in_eventStr) = 0;

private:
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
