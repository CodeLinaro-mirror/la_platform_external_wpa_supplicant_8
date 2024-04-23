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
#include <aidl/vendor/qti/hardware/wifi/supplicant/IVendorIfaceType.h>

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
class IVendorIfaceInfo {
public:
  ::aidl::vendor::qti::hardware::wifi::supplicant::IVendorIfaceType type = ::aidl::vendor::qti::hardware::wifi::supplicant::IVendorIfaceType(0);
  std::string name;

  inline bool operator!=(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) != std::tie(rhs.type, rhs.name);
  }
  inline bool operator<(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) < std::tie(rhs.type, rhs.name);
  }
  inline bool operator<=(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) <= std::tie(rhs.type, rhs.name);
  }
  inline bool operator==(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) == std::tie(rhs.type, rhs.name);
  }
  inline bool operator>(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) > std::tie(rhs.type, rhs.name);
  }
  inline bool operator>=(const IVendorIfaceInfo& rhs) const {
    return std::tie(type, name) >= std::tie(rhs.type, rhs.name);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
