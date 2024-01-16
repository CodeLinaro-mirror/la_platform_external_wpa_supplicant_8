/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class IfaceInfo {
public:
  ::aidl::android::hardware::wifi::supplicant::IfaceType type = ::aidl::android::hardware::wifi::supplicant::IfaceType(0);
  std::string name;

  inline bool operator!=(const IfaceInfo& rhs) const {
    return std::tie(type, name) != std::tie(rhs.type, rhs.name);
  }
  inline bool operator<(const IfaceInfo& rhs) const {
    return std::tie(type, name) < std::tie(rhs.type, rhs.name);
  }
  inline bool operator<=(const IfaceInfo& rhs) const {
    return std::tie(type, name) <= std::tie(rhs.type, rhs.name);
  }
  inline bool operator==(const IfaceInfo& rhs) const {
    return std::tie(type, name) == std::tie(rhs.type, rhs.name);
  }
  inline bool operator>(const IfaceInfo& rhs) const {
    return std::tie(type, name) > std::tie(rhs.type, rhs.name);
  }
  inline bool operator>=(const IfaceInfo& rhs) const {
    return std::tie(type, name) >= std::tie(rhs.type, rhs.name);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
