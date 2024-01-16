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
enum class DebugLevel : int32_t {
  EXCESSIVE = 0,
  MSGDUMP = 1,
  DEBUG = 2,
  INFO = 3,
  WARNING = 4,
  ERROR = 5,
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
[[nodiscard]] static inline std::string toString(DebugLevel val) {
  switch(val) {
  case DebugLevel::EXCESSIVE:
    return "EXCESSIVE";
  case DebugLevel::MSGDUMP:
    return "MSGDUMP";
  case DebugLevel::DEBUG:
    return "DEBUG";
  case DebugLevel::INFO:
    return "INFO";
  case DebugLevel::WARNING:
    return "WARNING";
  case DebugLevel::ERROR:
    return "ERROR";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
