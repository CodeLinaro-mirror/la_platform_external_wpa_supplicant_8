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
enum class LegacyMode : int32_t {
  UNKNOWN = 0,
  A_MODE = 1,
  B_MODE = 2,
  G_MODE = 3,
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
[[nodiscard]] static inline std::string toString(LegacyMode val) {
  switch(val) {
  case LegacyMode::UNKNOWN:
    return "UNKNOWN";
  case LegacyMode::A_MODE:
    return "A_MODE";
  case LegacyMode::B_MODE:
    return "B_MODE";
  case LegacyMode::G_MODE:
    return "G_MODE";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
