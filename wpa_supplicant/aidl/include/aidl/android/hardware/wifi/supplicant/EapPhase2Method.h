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
enum class EapPhase2Method : int32_t {
  NONE = 0,
  PAP = 1,
  MSPAP = 2,
  MSPAPV2 = 3,
  GTC = 4,
  SIM = 5,
  AKA = 6,
  AKA_PRIME = 7,
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
[[nodiscard]] static inline std::string toString(EapPhase2Method val) {
  switch(val) {
  case EapPhase2Method::NONE:
    return "NONE";
  case EapPhase2Method::PAP:
    return "PAP";
  case EapPhase2Method::MSPAP:
    return "MSPAP";
  case EapPhase2Method::MSPAPV2:
    return "MSPAPV2";
  case EapPhase2Method::GTC:
    return "GTC";
  case EapPhase2Method::SIM:
    return "SIM";
  case EapPhase2Method::AKA:
    return "AKA";
  case EapPhase2Method::AKA_PRIME:
    return "AKA_PRIME";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
