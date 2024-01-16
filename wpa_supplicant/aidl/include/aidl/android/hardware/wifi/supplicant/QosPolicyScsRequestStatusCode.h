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
enum class QosPolicyScsRequestStatusCode : int32_t {
  SENT = 0,
  ALREADY_ACTIVE = 1,
  NOT_EXIST = 2,
  INVALID = 3,
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
[[nodiscard]] static inline std::string toString(QosPolicyScsRequestStatusCode val) {
  switch(val) {
  case QosPolicyScsRequestStatusCode::SENT:
    return "SENT";
  case QosPolicyScsRequestStatusCode::ALREADY_ACTIVE:
    return "ALREADY_ACTIVE";
  case QosPolicyScsRequestStatusCode::NOT_EXIST:
    return "NOT_EXIST";
  case QosPolicyScsRequestStatusCode::INVALID:
    return "INVALID";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
