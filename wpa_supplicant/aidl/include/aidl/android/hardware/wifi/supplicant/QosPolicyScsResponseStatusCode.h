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
enum class QosPolicyScsResponseStatusCode : int32_t {
  SUCCESS = 0,
  TCLAS_REQUEST_DECLINED = 1,
  TCLAS_NOT_SUPPORTED_BY_AP = 2,
  TCLAS_INSUFFICIENT_RESOURCES = 3,
  TCLAS_RESOURCES_EXHAUSTED = 4,
  TCLAS_PROCESSING_TERMINATED_INSUFFICIENT_QOS = 5,
  TCLAS_PROCESSING_TERMINATED_POLICY_CONFLICT = 6,
  TCLAS_PROCESSING_TERMINATED = 7,
  TIMEOUT = 8,
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
[[nodiscard]] static inline std::string toString(QosPolicyScsResponseStatusCode val) {
  switch(val) {
  case QosPolicyScsResponseStatusCode::SUCCESS:
    return "SUCCESS";
  case QosPolicyScsResponseStatusCode::TCLAS_REQUEST_DECLINED:
    return "TCLAS_REQUEST_DECLINED";
  case QosPolicyScsResponseStatusCode::TCLAS_NOT_SUPPORTED_BY_AP:
    return "TCLAS_NOT_SUPPORTED_BY_AP";
  case QosPolicyScsResponseStatusCode::TCLAS_INSUFFICIENT_RESOURCES:
    return "TCLAS_INSUFFICIENT_RESOURCES";
  case QosPolicyScsResponseStatusCode::TCLAS_RESOURCES_EXHAUSTED:
    return "TCLAS_RESOURCES_EXHAUSTED";
  case QosPolicyScsResponseStatusCode::TCLAS_PROCESSING_TERMINATED_INSUFFICIENT_QOS:
    return "TCLAS_PROCESSING_TERMINATED_INSUFFICIENT_QOS";
  case QosPolicyScsResponseStatusCode::TCLAS_PROCESSING_TERMINATED_POLICY_CONFLICT:
    return "TCLAS_PROCESSING_TERMINATED_POLICY_CONFLICT";
  case QosPolicyScsResponseStatusCode::TCLAS_PROCESSING_TERMINATED:
    return "TCLAS_PROCESSING_TERMINATED";
  case QosPolicyScsResponseStatusCode::TIMEOUT:
    return "TIMEOUT";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
