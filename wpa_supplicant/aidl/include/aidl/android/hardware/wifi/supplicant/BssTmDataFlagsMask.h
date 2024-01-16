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
enum class BssTmDataFlagsMask : int32_t {
  WNM_MODE_PREFERRED_CANDIDATE_LIST_INCLUDED = 1,
  WNM_MODE_ABRIDGED = 2,
  WNM_MODE_DISASSOCIATION_IMMINENT = 4,
  WNM_MODE_BSS_TERMINATION_INCLUDED = 8,
  WNM_MODE_ESS_DISASSOCIATION_IMMINENT = 16,
  MBO_TRANSITION_REASON_CODE_INCLUDED = 32,
  MBO_ASSOC_RETRY_DELAY_INCLUDED = 64,
  MBO_CELLULAR_DATA_CONNECTION_PREFERENCE_INCLUDED = 128,
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
[[nodiscard]] static inline std::string toString(BssTmDataFlagsMask val) {
  switch(val) {
  case BssTmDataFlagsMask::WNM_MODE_PREFERRED_CANDIDATE_LIST_INCLUDED:
    return "WNM_MODE_PREFERRED_CANDIDATE_LIST_INCLUDED";
  case BssTmDataFlagsMask::WNM_MODE_ABRIDGED:
    return "WNM_MODE_ABRIDGED";
  case BssTmDataFlagsMask::WNM_MODE_DISASSOCIATION_IMMINENT:
    return "WNM_MODE_DISASSOCIATION_IMMINENT";
  case BssTmDataFlagsMask::WNM_MODE_BSS_TERMINATION_INCLUDED:
    return "WNM_MODE_BSS_TERMINATION_INCLUDED";
  case BssTmDataFlagsMask::WNM_MODE_ESS_DISASSOCIATION_IMMINENT:
    return "WNM_MODE_ESS_DISASSOCIATION_IMMINENT";
  case BssTmDataFlagsMask::MBO_TRANSITION_REASON_CODE_INCLUDED:
    return "MBO_TRANSITION_REASON_CODE_INCLUDED";
  case BssTmDataFlagsMask::MBO_ASSOC_RETRY_DELAY_INCLUDED:
    return "MBO_ASSOC_RETRY_DELAY_INCLUDED";
  case BssTmDataFlagsMask::MBO_CELLULAR_DATA_CONNECTION_PREFERENCE_INCLUDED:
    return "MBO_CELLULAR_DATA_CONNECTION_PREFERENCE_INCLUDED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
