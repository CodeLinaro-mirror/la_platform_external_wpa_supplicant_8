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
enum class BssTmStatusCode : int8_t {
  ACCEPT = 0,
  REJECT_UNSPECIFIED = 1,
  REJECT_INSUFFICIENT_BEACON = 2,
  REJECT_INSUFFICIENT_CAPABITY = 3,
  REJECT_BSS_TERMINATION_UNDESIRED = 4,
  REJECT_BSS_TERMINATION_DELAY_REQUEST = 5,
  REJECT_STA_CANDIDATE_LIST_PROVIDED = 6,
  REJECT_NO_SUITABLE_CANDIDATES = 7,
  REJECT_LEAVING_ESS = 8,
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
[[nodiscard]] static inline std::string toString(BssTmStatusCode val) {
  switch(val) {
  case BssTmStatusCode::ACCEPT:
    return "ACCEPT";
  case BssTmStatusCode::REJECT_UNSPECIFIED:
    return "REJECT_UNSPECIFIED";
  case BssTmStatusCode::REJECT_INSUFFICIENT_BEACON:
    return "REJECT_INSUFFICIENT_BEACON";
  case BssTmStatusCode::REJECT_INSUFFICIENT_CAPABITY:
    return "REJECT_INSUFFICIENT_CAPABITY";
  case BssTmStatusCode::REJECT_BSS_TERMINATION_UNDESIRED:
    return "REJECT_BSS_TERMINATION_UNDESIRED";
  case BssTmStatusCode::REJECT_BSS_TERMINATION_DELAY_REQUEST:
    return "REJECT_BSS_TERMINATION_DELAY_REQUEST";
  case BssTmStatusCode::REJECT_STA_CANDIDATE_LIST_PROVIDED:
    return "REJECT_STA_CANDIDATE_LIST_PROVIDED";
  case BssTmStatusCode::REJECT_NO_SUITABLE_CANDIDATES:
    return "REJECT_NO_SUITABLE_CANDIDATES";
  case BssTmStatusCode::REJECT_LEAVING_ESS:
    return "REJECT_LEAVING_ESS";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
