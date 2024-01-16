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
enum class StaIfaceCallbackState : int32_t {
  DISCONNECTED = 0,
  IFACE_DISABLED = 1,
  INACTIVE = 2,
  SCANNING = 3,
  AUTHENTICATING = 4,
  ASSOCIATING = 5,
  ASSOCIATED = 6,
  FOURWAY_HANDSHAKE = 7,
  GROUP_HANDSHAKE = 8,
  COMPLETED = 9,
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
[[nodiscard]] static inline std::string toString(StaIfaceCallbackState val) {
  switch(val) {
  case StaIfaceCallbackState::DISCONNECTED:
    return "DISCONNECTED";
  case StaIfaceCallbackState::IFACE_DISABLED:
    return "IFACE_DISABLED";
  case StaIfaceCallbackState::INACTIVE:
    return "INACTIVE";
  case StaIfaceCallbackState::SCANNING:
    return "SCANNING";
  case StaIfaceCallbackState::AUTHENTICATING:
    return "AUTHENTICATING";
  case StaIfaceCallbackState::ASSOCIATING:
    return "ASSOCIATING";
  case StaIfaceCallbackState::ASSOCIATED:
    return "ASSOCIATED";
  case StaIfaceCallbackState::FOURWAY_HANDSHAKE:
    return "FOURWAY_HANDSHAKE";
  case StaIfaceCallbackState::GROUP_HANDSHAKE:
    return "GROUP_HANDSHAKE";
  case StaIfaceCallbackState::COMPLETED:
    return "COMPLETED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
