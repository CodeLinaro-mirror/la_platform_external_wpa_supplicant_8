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
#include <aidl/vendor/qti/hardware/wifi/supplicant/ISupplicantVendorStaIfaceCallback.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::vendor::qti::hardware::wifi::supplicant {
class ISupplicantVendorStaIfaceCallback;
}  // namespace aidl::vendor::qti::hardware::wifi::supplicant
namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantVendorStaIface : public ::ndk::ICInterface {
public:
//  ISupplicantVendorStaIface();
//  virtual ~ISupplicantVendorStaIface();

  static const int32_t version = 2;

  virtual ::ndk::ScopedAStatus doDriverCmd(const std::string& in_command, std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus registerSupplicantVendorStaIfaceCallback(const std::shared_ptr<::aidl::vendor::qti::hardware::wifi::supplicant::ISupplicantVendorStaIfaceCallback>& in_callback) = 0;

private:
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
