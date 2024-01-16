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
enum class PairwiseCipherMask : int32_t {
  NONE = 1,
  TKIP = 8,
  CCMP = 16,
  GCMP_128 = 64,
  SMS4 = 128,
  GCMP_256 = 256,
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
[[nodiscard]] static inline std::string toString(PairwiseCipherMask val) {
  switch(val) {
  case PairwiseCipherMask::NONE:
    return "NONE";
  case PairwiseCipherMask::TKIP:
    return "TKIP";
  case PairwiseCipherMask::CCMP:
    return "CCMP";
  case PairwiseCipherMask::GCMP_128:
    return "GCMP_128";
  case PairwiseCipherMask::SMS4:
    return "SMS4";
  case PairwiseCipherMask::GCMP_256:
    return "GCMP_256";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
