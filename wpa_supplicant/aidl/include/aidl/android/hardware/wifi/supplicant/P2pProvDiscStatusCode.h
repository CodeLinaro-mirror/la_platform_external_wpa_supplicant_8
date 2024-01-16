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
enum class P2pProvDiscStatusCode : int8_t {
  SUCCESS = 0,
  TIMEOUT = 1,
  REJECTED = 2,
  TIMEOUT_JOIN = 3,
  INFO_UNAVAILABLE = 4,
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
[[nodiscard]] static inline std::string toString(P2pProvDiscStatusCode val) {
  switch(val) {
  case P2pProvDiscStatusCode::SUCCESS:
    return "SUCCESS";
  case P2pProvDiscStatusCode::TIMEOUT:
    return "TIMEOUT";
  case P2pProvDiscStatusCode::REJECTED:
    return "REJECTED";
  case P2pProvDiscStatusCode::TIMEOUT_JOIN:
    return "TIMEOUT_JOIN";
  case P2pProvDiscStatusCode::INFO_UNAVAILABLE:
    return "INFO_UNAVAILABLE";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
