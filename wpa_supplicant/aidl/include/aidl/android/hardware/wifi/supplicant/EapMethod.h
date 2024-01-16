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
enum class EapMethod : int32_t {
  PEAP = 0,
  TLS = 1,
  TTLS = 2,
  PWD = 3,
  SIM = 4,
  AKA = 5,
  AKA_PRIME = 6,
  WFA_UNAUTH_TLS = 7,
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
[[nodiscard]] static inline std::string toString(EapMethod val) {
  switch(val) {
  case EapMethod::PEAP:
    return "PEAP";
  case EapMethod::TLS:
    return "TLS";
  case EapMethod::TTLS:
    return "TTLS";
  case EapMethod::PWD:
    return "PWD";
  case EapMethod::SIM:
    return "SIM";
  case EapMethod::AKA:
    return "AKA";
  case EapMethod::AKA_PRIME:
    return "AKA_PRIME";
  case EapMethod::WFA_UNAUTH_TLS:
    return "WFA_UNAUTH_TLS";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
