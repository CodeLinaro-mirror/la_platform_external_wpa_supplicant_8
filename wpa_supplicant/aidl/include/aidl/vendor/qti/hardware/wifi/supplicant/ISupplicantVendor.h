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
#include <aidl/vendor/qti/hardware/wifi/supplicant/ISupplicantVendorStaIface.h>
#include <aidl/vendor/qti/hardware/wifi/supplicant/IVendorIfaceInfo.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::vendor::qti::hardware::wifi::supplicant {
class ISupplicantVendorStaIface;
class IVendorIfaceInfo;
}  // namespace aidl::vendor::qti::hardware::wifi::supplicant
namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantVendor : public ::ndk::ICInterface {
public:
//  ISupplicantVendor();
//  virtual ~ISupplicantVendor();

  static const int32_t version = 2;

  static const std::shared_ptr<ISupplicantVendor>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus getVendorInterface(const ::aidl::vendor::qti::hardware::wifi::supplicant::IVendorIfaceInfo& in_ifaceInfo, std::shared_ptr<::aidl::vendor::qti::hardware::wifi::supplicant::ISupplicantVendorStaIface>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus listVendorInterfaces(std::vector<::aidl::vendor::qti::hardware::wifi::supplicant::IVendorIfaceInfo>* _aidl_return) = 0;

private:
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
