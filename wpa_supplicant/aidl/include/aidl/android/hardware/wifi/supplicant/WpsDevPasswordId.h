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
enum class WpsDevPasswordId : int32_t {
  DEFAULT = 0,
  USER_SPECIFIED = 1,
  MACHINE_SPECIFIED = 2,
  REKEY = 3,
  PUSHBUTTON = 4,
  REGISTRAR_SPECIFIED = 5,
  NFC_CONNECTION_HANDOVER = 7,
  P2PS_DEFAULT = 8,
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
[[nodiscard]] static inline std::string toString(WpsDevPasswordId val) {
  switch(val) {
  case WpsDevPasswordId::DEFAULT:
    return "DEFAULT";
  case WpsDevPasswordId::USER_SPECIFIED:
    return "USER_SPECIFIED";
  case WpsDevPasswordId::MACHINE_SPECIFIED:
    return "MACHINE_SPECIFIED";
  case WpsDevPasswordId::REKEY:
    return "REKEY";
  case WpsDevPasswordId::PUSHBUTTON:
    return "PUSHBUTTON";
  case WpsDevPasswordId::REGISTRAR_SPECIFIED:
    return "REGISTRAR_SPECIFIED";
  case WpsDevPasswordId::NFC_CONNECTION_HANDOVER:
    return "NFC_CONNECTION_HANDOVER";
  case WpsDevPasswordId::P2PS_DEFAULT:
    return "P2PS_DEFAULT";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
