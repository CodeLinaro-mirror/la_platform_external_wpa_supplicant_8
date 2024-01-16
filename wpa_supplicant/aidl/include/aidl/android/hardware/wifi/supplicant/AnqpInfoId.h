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
enum class AnqpInfoId : int32_t {
  VENUE_NAME = 258,
  ROAMING_CONSORTIUM = 261,
  IP_ADDR_TYPE_AVAILABILITY = 262,
  NAI_REALM = 263,
  ANQP_3GPP_CELLULAR_NETWORK = 264,
  DOMAIN_NAME = 268,
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
[[nodiscard]] static inline std::string toString(AnqpInfoId val) {
  switch(val) {
  case AnqpInfoId::VENUE_NAME:
    return "VENUE_NAME";
  case AnqpInfoId::ROAMING_CONSORTIUM:
    return "ROAMING_CONSORTIUM";
  case AnqpInfoId::IP_ADDR_TYPE_AVAILABILITY:
    return "IP_ADDR_TYPE_AVAILABILITY";
  case AnqpInfoId::NAI_REALM:
    return "NAI_REALM";
  case AnqpInfoId::ANQP_3GPP_CELLULAR_NETWORK:
    return "ANQP_3GPP_CELLULAR_NETWORK";
  case AnqpInfoId::DOMAIN_NAME:
    return "DOMAIN_NAME";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
