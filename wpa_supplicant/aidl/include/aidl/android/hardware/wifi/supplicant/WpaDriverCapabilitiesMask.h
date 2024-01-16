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
enum class WpaDriverCapabilitiesMask : int32_t {
  MBO = 1,
  OCE = 2,
  SAE_PK = 4,
  WFD_R2 = 8,
  TRUST_ON_FIRST_USE = 16,
  SET_TLS_MINIMUM_VERSION = 32,
  TLS_V1_3 = 64,
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
[[nodiscard]] static inline std::string toString(WpaDriverCapabilitiesMask val) {
  switch(val) {
  case WpaDriverCapabilitiesMask::MBO:
    return "MBO";
  case WpaDriverCapabilitiesMask::OCE:
    return "OCE";
  case WpaDriverCapabilitiesMask::SAE_PK:
    return "SAE_PK";
  case WpaDriverCapabilitiesMask::WFD_R2:
    return "WFD_R2";
  case WpaDriverCapabilitiesMask::TRUST_ON_FIRST_USE:
    return "TRUST_ON_FIRST_USE";
  case WpaDriverCapabilitiesMask::SET_TLS_MINIMUM_VERSION:
    return "SET_TLS_MINIMUM_VERSION";
  case WpaDriverCapabilitiesMask::TLS_V1_3:
    return "TLS_V1_3";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
