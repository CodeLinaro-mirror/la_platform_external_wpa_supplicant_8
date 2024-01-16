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
class DppResponderBootstrapInfo {
public:
  int32_t bootstrapId = 0;
  int32_t listenChannel = 0;
  std::string uri;

  inline bool operator!=(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) != std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
  inline bool operator<(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) < std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
  inline bool operator<=(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) <= std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
  inline bool operator==(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) == std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
  inline bool operator>(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) > std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
  inline bool operator>=(const DppResponderBootstrapInfo& rhs) const {
    return std::tie(bootstrapId, listenChannel, uri) >= std::tie(rhs.bootstrapId, rhs.listenChannel, rhs.uri);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
