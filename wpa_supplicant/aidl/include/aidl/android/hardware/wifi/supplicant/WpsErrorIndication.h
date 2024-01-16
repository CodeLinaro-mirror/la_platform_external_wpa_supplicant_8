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
enum class WpsErrorIndication : int32_t {
  NO_ERROR = 0,
  SECURITY_TKIP_ONLY_PROHIBITED = 1,
  SECURITY_WEP_PROHIBITED = 2,
  AUTH_FAILURE = 3,
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
[[nodiscard]] static inline std::string toString(WpsErrorIndication val) {
  switch(val) {
  case WpsErrorIndication::NO_ERROR:
    return "NO_ERROR";
  case WpsErrorIndication::SECURITY_TKIP_ONLY_PROHIBITED:
    return "SECURITY_TKIP_ONLY_PROHIBITED";
  case WpsErrorIndication::SECURITY_WEP_PROHIBITED:
    return "SECURITY_WEP_PROHIBITED";
  case WpsErrorIndication::AUTH_FAILURE:
    return "AUTH_FAILURE";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
