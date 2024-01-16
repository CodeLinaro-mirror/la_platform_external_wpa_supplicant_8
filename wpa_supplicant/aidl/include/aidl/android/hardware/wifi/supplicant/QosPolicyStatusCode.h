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
enum class QosPolicyStatusCode : int8_t {
  QOS_POLICY_SUCCESS = 0,
  QOS_POLICY_REQUEST_DECLINED = 1,
  QOS_POLICY_CLASSIFIER_NOT_SUPPORTED = 2,
  QOS_POLICY_INSUFFICIENT_RESOURCES = 3,
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
[[nodiscard]] static inline std::string toString(QosPolicyStatusCode val) {
  switch(val) {
  case QosPolicyStatusCode::QOS_POLICY_SUCCESS:
    return "QOS_POLICY_SUCCESS";
  case QosPolicyStatusCode::QOS_POLICY_REQUEST_DECLINED:
    return "QOS_POLICY_REQUEST_DECLINED";
  case QosPolicyStatusCode::QOS_POLICY_CLASSIFIER_NOT_SUPPORTED:
    return "QOS_POLICY_CLASSIFIER_NOT_SUPPORTED";
  case QosPolicyStatusCode::QOS_POLICY_INSUFFICIENT_RESOURCES:
    return "QOS_POLICY_INSUFFICIENT_RESOURCES";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
