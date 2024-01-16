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
enum class BssidChangeReason : int8_t {
  ASSOC_START = 0,
  ASSOC_COMPLETE = 1,
  DISASSOC = 2,
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
[[nodiscard]] static inline std::string toString(BssidChangeReason val) {
  switch(val) {
  case BssidChangeReason::ASSOC_START:
    return "ASSOC_START";
  case BssidChangeReason::ASSOC_COMPLETE:
    return "ASSOC_COMPLETE";
  case BssidChangeReason::DISASSOC:
    return "DISASSOC";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
