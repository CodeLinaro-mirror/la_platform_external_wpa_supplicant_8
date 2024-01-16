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
enum class GroupMgmtCipherMask : int32_t {
  BIP_GMAC_128 = 2048,
  BIP_GMAC_256 = 4096,
  BIP_CMAC_256 = 8192,
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
[[nodiscard]] static inline std::string toString(GroupMgmtCipherMask val) {
  switch(val) {
  case GroupMgmtCipherMask::BIP_GMAC_128:
    return "BIP_GMAC_128";
  case GroupMgmtCipherMask::BIP_GMAC_256:
    return "BIP_GMAC_256";
  case GroupMgmtCipherMask::BIP_CMAC_256:
    return "BIP_CMAC_256";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
