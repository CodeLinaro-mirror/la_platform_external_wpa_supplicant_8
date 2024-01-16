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
enum class AuxiliarySupplicantEventCode : int32_t {
  EAP_METHOD_SELECTED = 0,
  SSID_TEMP_DISABLED = 1,
  OPEN_SSL_FAILURE = 2,
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
[[nodiscard]] static inline std::string toString(AuxiliarySupplicantEventCode val) {
  switch(val) {
  case AuxiliarySupplicantEventCode::EAP_METHOD_SELECTED:
    return "EAP_METHOD_SELECTED";
  case AuxiliarySupplicantEventCode::SSID_TEMP_DISABLED:
    return "SSID_TEMP_DISABLED";
  case AuxiliarySupplicantEventCode::OPEN_SSL_FAILURE:
    return "OPEN_SSL_FAILURE";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
