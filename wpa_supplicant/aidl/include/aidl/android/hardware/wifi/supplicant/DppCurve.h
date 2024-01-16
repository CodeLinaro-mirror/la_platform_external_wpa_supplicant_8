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
enum class DppCurve : int32_t {
  PRIME256V1 = 0,
  SECP384R1 = 1,
  SECP521R1 = 2,
  BRAINPOOLP256R1 = 3,
  BRAINPOOLP384R1 = 4,
  BRAINPOOLP512R1 = 5,
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
[[nodiscard]] static inline std::string toString(DppCurve val) {
  switch(val) {
  case DppCurve::PRIME256V1:
    return "PRIME256V1";
  case DppCurve::SECP384R1:
    return "SECP384R1";
  case DppCurve::SECP521R1:
    return "SECP521R1";
  case DppCurve::BRAINPOOLP256R1:
    return "BRAINPOOLP256R1";
  case DppCurve::BRAINPOOLP384R1:
    return "BRAINPOOLP384R1";
  case DppCurve::BRAINPOOLP512R1:
    return "BRAINPOOLP512R1";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
