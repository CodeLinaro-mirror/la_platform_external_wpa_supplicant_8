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
enum class GroupCipherMask : int32_t {
  WEP40 = 2,
  WEP104 = 4,
  TKIP = 8,
  CCMP = 16,
  GTK_NOT_USED = 16384,
  GCMP_256 = 256,
  SMS4 = 128,
  GCMP_128 = 64,
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
[[nodiscard]] static inline std::string toString(GroupCipherMask val) {
  switch(val) {
  case GroupCipherMask::WEP40:
    return "WEP40";
  case GroupCipherMask::WEP104:
    return "WEP104";
  case GroupCipherMask::TKIP:
    return "TKIP";
  case GroupCipherMask::CCMP:
    return "CCMP";
  case GroupCipherMask::GTK_NOT_USED:
    return "GTK_NOT_USED";
  case GroupCipherMask::GCMP_256:
    return "GCMP_256";
  case GroupCipherMask::SMS4:
    return "SMS4";
  case GroupCipherMask::GCMP_128:
    return "GCMP_128";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
