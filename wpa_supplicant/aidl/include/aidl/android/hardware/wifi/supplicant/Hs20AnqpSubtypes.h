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
enum class Hs20AnqpSubtypes : int32_t {
  OPERATOR_FRIENDLY_NAME = 3,
  WAN_METRICS = 4,
  CONNECTION_CAPABILITY = 5,
  OSU_PROVIDERS_LIST = 8,
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
[[nodiscard]] static inline std::string toString(Hs20AnqpSubtypes val) {
  switch(val) {
  case Hs20AnqpSubtypes::OPERATOR_FRIENDLY_NAME:
    return "OPERATOR_FRIENDLY_NAME";
  case Hs20AnqpSubtypes::WAN_METRICS:
    return "WAN_METRICS";
  case Hs20AnqpSubtypes::CONNECTION_CAPABILITY:
    return "CONNECTION_CAPABILITY";
  case Hs20AnqpSubtypes::OSU_PROVIDERS_LIST:
    return "OSU_PROVIDERS_LIST";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
