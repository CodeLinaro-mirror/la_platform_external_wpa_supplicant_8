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
enum class AuthAlgMask : int32_t {
  OPEN = 1,
  SHARED = 2,
  LEAP = 4,
  SAE = 16,
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
[[nodiscard]] static inline std::string toString(AuthAlgMask val) {
  switch(val) {
  case AuthAlgMask::OPEN:
    return "OPEN";
  case AuthAlgMask::SHARED:
    return "SHARED";
  case AuthAlgMask::LEAP:
    return "LEAP";
  case AuthAlgMask::SAE:
    return "SAE";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
