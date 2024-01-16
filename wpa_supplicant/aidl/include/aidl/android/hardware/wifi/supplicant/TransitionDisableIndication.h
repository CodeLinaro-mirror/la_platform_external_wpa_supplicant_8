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
enum class TransitionDisableIndication : int32_t {
  USE_WPA3_PERSONAL = 1,
  USE_SAE_PK = 2,
  USE_WPA3_ENTERPRISE = 4,
  USE_ENHANCED_OPEN = 8,
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
[[nodiscard]] static inline std::string toString(TransitionDisableIndication val) {
  switch(val) {
  case TransitionDisableIndication::USE_WPA3_PERSONAL:
    return "USE_WPA3_PERSONAL";
  case TransitionDisableIndication::USE_SAE_PK:
    return "USE_SAE_PK";
  case TransitionDisableIndication::USE_WPA3_ENTERPRISE:
    return "USE_WPA3_ENTERPRISE";
  case TransitionDisableIndication::USE_ENHANCED_OPEN:
    return "USE_ENHANCED_OPEN";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
