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
enum class DppNetRole : int32_t {
  STA = 0,
  AP = 1,
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
[[nodiscard]] static inline std::string toString(DppNetRole val) {
  switch(val) {
  case DppNetRole::STA:
    return "STA";
  case DppNetRole::AP:
    return "AP";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
