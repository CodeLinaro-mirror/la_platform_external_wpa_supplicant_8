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
enum class QosPolicyClassifierParamsMask : int32_t {
  SRC_IP = 1,
  DST_IP = 2,
  SRC_PORT = 4,
  DST_PORT_RANGE = 8,
  PROTOCOL_NEXT_HEADER = 16,
  FLOW_LABEL = 32,
  DOMAIN_NAME = 64,
  DSCP = 128,
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
[[nodiscard]] static inline std::string toString(QosPolicyClassifierParamsMask val) {
  switch(val) {
  case QosPolicyClassifierParamsMask::SRC_IP:
    return "SRC_IP";
  case QosPolicyClassifierParamsMask::DST_IP:
    return "DST_IP";
  case QosPolicyClassifierParamsMask::SRC_PORT:
    return "SRC_PORT";
  case QosPolicyClassifierParamsMask::DST_PORT_RANGE:
    return "DST_PORT_RANGE";
  case QosPolicyClassifierParamsMask::PROTOCOL_NEXT_HEADER:
    return "PROTOCOL_NEXT_HEADER";
  case QosPolicyClassifierParamsMask::FLOW_LABEL:
    return "FLOW_LABEL";
  case QosPolicyClassifierParamsMask::DOMAIN_NAME:
    return "DOMAIN_NAME";
  case QosPolicyClassifierParamsMask::DSCP:
    return "DSCP";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
