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
enum class MboAssocDisallowedReasonCode : int8_t {
  RESERVED = 0,
  UNSPECIFIED = 1,
  MAX_NUM_STA_ASSOCIATED = 2,
  AIR_INTERFACE_OVERLOADED = 3,
  AUTH_SERVER_OVERLOADED = 4,
  INSUFFICIENT_RSSI = 5,
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
[[nodiscard]] static inline std::string toString(MboAssocDisallowedReasonCode val) {
  switch(val) {
  case MboAssocDisallowedReasonCode::RESERVED:
    return "RESERVED";
  case MboAssocDisallowedReasonCode::UNSPECIFIED:
    return "UNSPECIFIED";
  case MboAssocDisallowedReasonCode::MAX_NUM_STA_ASSOCIATED:
    return "MAX_NUM_STA_ASSOCIATED";
  case MboAssocDisallowedReasonCode::AIR_INTERFACE_OVERLOADED:
    return "AIR_INTERFACE_OVERLOADED";
  case MboAssocDisallowedReasonCode::AUTH_SERVER_OVERLOADED:
    return "AUTH_SERVER_OVERLOADED";
  case MboAssocDisallowedReasonCode::INSUFFICIENT_RSSI:
    return "INSUFFICIENT_RSSI";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
