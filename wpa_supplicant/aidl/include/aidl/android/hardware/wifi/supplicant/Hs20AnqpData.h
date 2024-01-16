/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

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
class Hs20AnqpData {
public:
  std::vector<uint8_t> operatorFriendlyName;
  std::vector<uint8_t> wanMetrics;
  std::vector<uint8_t> connectionCapability;
  std::vector<uint8_t> osuProvidersList;

  inline bool operator!=(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) != std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
  inline bool operator<(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) < std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
  inline bool operator<=(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) <= std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
  inline bool operator==(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) == std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
  inline bool operator>(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) > std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
  inline bool operator>=(const Hs20AnqpData& rhs) const {
    return std::tie(operatorFriendlyName, wanMetrics, connectionCapability, osuProvidersList) >= std::tie(rhs.operatorFriendlyName, rhs.wanMetrics, rhs.connectionCapability, rhs.osuProvidersList);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
