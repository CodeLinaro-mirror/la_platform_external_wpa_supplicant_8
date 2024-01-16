/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
enum class WifiTechnology : int32_t {
  UNKNOWN = 0,
  LEGACY = 1,
  HT = 2,
  VHT = 3,
  HE = 4,
  EHT = 5,
};

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
[[nodiscard]] static inline std::string toString(WifiTechnology val) {
  switch(val) {
  case WifiTechnology::UNKNOWN:
    return "UNKNOWN";
  case WifiTechnology::LEGACY:
    return "LEGACY";
  case WifiTechnology::HT:
    return "HT";
  case WifiTechnology::VHT:
    return "VHT";
  case WifiTechnology::HE:
    return "HE";
  case WifiTechnology::EHT:
    return "EHT";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
