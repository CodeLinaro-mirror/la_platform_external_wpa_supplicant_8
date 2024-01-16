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
enum class WpsConfigMethods : int32_t {
  USBA = 1,
  ETHERNET = 2,
  LABEL = 4,
  DISPLAY = 8,
  EXT_NFC_TOKEN = 16,
  INT_NFC_TOKEN = 32,
  NFC_INTERFACE = 64,
  PUSHBUTTON = 128,
  KEYPAD = 256,
  VIRT_PUSHBUTTON = 640,
  PHY_PUSHBUTTON = 1152,
  P2PS = 4096,
  VIRT_DISPLAY = 8200,
  PHY_DISPLAY = 16392,
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
[[nodiscard]] static inline std::string toString(WpsConfigMethods val) {
  switch(val) {
  case WpsConfigMethods::USBA:
    return "USBA";
  case WpsConfigMethods::ETHERNET:
    return "ETHERNET";
  case WpsConfigMethods::LABEL:
    return "LABEL";
  case WpsConfigMethods::DISPLAY:
    return "DISPLAY";
  case WpsConfigMethods::EXT_NFC_TOKEN:
    return "EXT_NFC_TOKEN";
  case WpsConfigMethods::INT_NFC_TOKEN:
    return "INT_NFC_TOKEN";
  case WpsConfigMethods::NFC_INTERFACE:
    return "NFC_INTERFACE";
  case WpsConfigMethods::PUSHBUTTON:
    return "PUSHBUTTON";
  case WpsConfigMethods::KEYPAD:
    return "KEYPAD";
  case WpsConfigMethods::VIRT_PUSHBUTTON:
    return "VIRT_PUSHBUTTON";
  case WpsConfigMethods::PHY_PUSHBUTTON:
    return "PHY_PUSHBUTTON";
  case WpsConfigMethods::P2PS:
    return "P2PS";
  case WpsConfigMethods::VIRT_DISPLAY:
    return "VIRT_DISPLAY";
  case WpsConfigMethods::PHY_DISPLAY:
    return "PHY_DISPLAY";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
