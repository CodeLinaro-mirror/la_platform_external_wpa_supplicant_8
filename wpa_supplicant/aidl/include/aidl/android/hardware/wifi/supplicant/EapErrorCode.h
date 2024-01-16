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
enum class EapErrorCode : int32_t {
  SIM_GENERAL_FAILURE_AFTER_AUTH = 0,
  SIM_TEMPORARILY_DENIED = 1026,
  SIM_NOT_SUBSCRIBED = 1031,
  SIM_GENERAL_FAILURE_BEFORE_AUTH = 16384,
  SIM_VENDOR_SPECIFIC_EXPIRED_CERT = 16385,
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
[[nodiscard]] static inline std::string toString(EapErrorCode val) {
  switch(val) {
  case EapErrorCode::SIM_GENERAL_FAILURE_AFTER_AUTH:
    return "SIM_GENERAL_FAILURE_AFTER_AUTH";
  case EapErrorCode::SIM_TEMPORARILY_DENIED:
    return "SIM_TEMPORARILY_DENIED";
  case EapErrorCode::SIM_NOT_SUBSCRIBED:
    return "SIM_NOT_SUBSCRIBED";
  case EapErrorCode::SIM_GENERAL_FAILURE_BEFORE_AUTH:
    return "SIM_GENERAL_FAILURE_BEFORE_AUTH";
  case EapErrorCode::SIM_VENDOR_SPECIFIC_EXPIRED_CERT:
    return "SIM_VENDOR_SPECIFIC_EXPIRED_CERT";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
