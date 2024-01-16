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
class PortRange {
public:
  int32_t startPort = 0;
  int32_t endPort = 0;

  inline bool operator!=(const PortRange& rhs) const {
    return std::tie(startPort, endPort) != std::tie(rhs.startPort, rhs.endPort);
  }
  inline bool operator<(const PortRange& rhs) const {
    return std::tie(startPort, endPort) < std::tie(rhs.startPort, rhs.endPort);
  }
  inline bool operator<=(const PortRange& rhs) const {
    return std::tie(startPort, endPort) <= std::tie(rhs.startPort, rhs.endPort);
  }
  inline bool operator==(const PortRange& rhs) const {
    return std::tie(startPort, endPort) == std::tie(rhs.startPort, rhs.endPort);
  }
  inline bool operator>(const PortRange& rhs) const {
    return std::tie(startPort, endPort) > std::tie(rhs.startPort, rhs.endPort);
  }
  inline bool operator>=(const PortRange& rhs) const {
    return std::tie(startPort, endPort) >= std::tie(rhs.startPort, rhs.endPort);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
