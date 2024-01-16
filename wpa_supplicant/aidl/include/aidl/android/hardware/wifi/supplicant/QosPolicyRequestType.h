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
enum class QosPolicyRequestType : int8_t {
  QOS_POLICY_ADD = 0,
  QOS_POLICY_REMOVE = 1,
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
[[nodiscard]] static inline std::string toString(QosPolicyRequestType val) {
  switch(val) {
  case QosPolicyRequestType::QOS_POLICY_ADD:
    return "QOS_POLICY_ADD";
  case QosPolicyRequestType::QOS_POLICY_REMOVE:
    return "QOS_POLICY_REMOVE";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
