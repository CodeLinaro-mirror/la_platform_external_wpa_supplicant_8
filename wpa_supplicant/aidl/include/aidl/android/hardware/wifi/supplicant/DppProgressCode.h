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
enum class DppProgressCode : int32_t {
  AUTHENTICATION_SUCCESS = 0,
  RESPONSE_PENDING = 1,
  CONFIGURATION_SENT_WAITING_RESPONSE = 2,
  CONFIGURATION_ACCEPTED = 3,
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
[[nodiscard]] static inline std::string toString(DppProgressCode val) {
  switch(val) {
  case DppProgressCode::AUTHENTICATION_SUCCESS:
    return "AUTHENTICATION_SUCCESS";
  case DppProgressCode::RESPONSE_PENDING:
    return "RESPONSE_PENDING";
  case DppProgressCode::CONFIGURATION_SENT_WAITING_RESPONSE:
    return "CONFIGURATION_SENT_WAITING_RESPONSE";
  case DppProgressCode::CONFIGURATION_ACCEPTED:
    return "CONFIGURATION_ACCEPTED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
