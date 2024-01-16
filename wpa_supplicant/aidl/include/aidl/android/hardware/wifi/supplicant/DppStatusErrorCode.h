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
enum class DppStatusErrorCode : int32_t {
  UNKNOWN = -1,
  SUCCESS = 0,
  NOT_COMPATIBLE = 1,
  AUTH_FAILURE = 2,
  UNWRAP_FAILURE = 3,
  BAD_GROUP = 4,
  CONFIGURE_FAILURE = 5,
  RESPONSE_PENDING = 6,
  INVALID_CONNECTOR = 7,
  NO_MATCH = 8,
  CONFIG_REJECTED = 9,
  NO_AP = 10,
  CONFIGURE_PENDING = 11,
  CSR_NEEDED = 12,
  CSR_BAD = 13,
  NEW_KEY_NEEDED = 14,
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
[[nodiscard]] static inline std::string toString(DppStatusErrorCode val) {
  switch(val) {
  case DppStatusErrorCode::UNKNOWN:
    return "UNKNOWN";
  case DppStatusErrorCode::SUCCESS:
    return "SUCCESS";
  case DppStatusErrorCode::NOT_COMPATIBLE:
    return "NOT_COMPATIBLE";
  case DppStatusErrorCode::AUTH_FAILURE:
    return "AUTH_FAILURE";
  case DppStatusErrorCode::UNWRAP_FAILURE:
    return "UNWRAP_FAILURE";
  case DppStatusErrorCode::BAD_GROUP:
    return "BAD_GROUP";
  case DppStatusErrorCode::CONFIGURE_FAILURE:
    return "CONFIGURE_FAILURE";
  case DppStatusErrorCode::RESPONSE_PENDING:
    return "RESPONSE_PENDING";
  case DppStatusErrorCode::INVALID_CONNECTOR:
    return "INVALID_CONNECTOR";
  case DppStatusErrorCode::NO_MATCH:
    return "NO_MATCH";
  case DppStatusErrorCode::CONFIG_REJECTED:
    return "CONFIG_REJECTED";
  case DppStatusErrorCode::NO_AP:
    return "NO_AP";
  case DppStatusErrorCode::CONFIGURE_PENDING:
    return "CONFIGURE_PENDING";
  case DppStatusErrorCode::CSR_NEEDED:
    return "CSR_NEEDED";
  case DppStatusErrorCode::CSR_BAD:
    return "CSR_BAD";
  case DppStatusErrorCode::NEW_KEY_NEEDED:
    return "NEW_KEY_NEEDED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
