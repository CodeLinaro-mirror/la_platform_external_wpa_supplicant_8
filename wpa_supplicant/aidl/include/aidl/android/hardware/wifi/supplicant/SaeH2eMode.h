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
enum class SaeH2eMode : int8_t {
  DISABLED = 0,
  H2E_OPTIONAL = 1,
  H2E_MANDATORY = 2,
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
[[nodiscard]] static inline std::string toString(SaeH2eMode val) {
  switch(val) {
  case SaeH2eMode::DISABLED:
    return "DISABLED";
  case SaeH2eMode::H2E_OPTIONAL:
    return "H2E_OPTIONAL";
  case SaeH2eMode::H2E_MANDATORY:
    return "H2E_MANDATORY";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
