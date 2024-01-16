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
class DppConnectionKeys {
public:
  std::vector<uint8_t> connector;
  std::vector<uint8_t> cSign;
  std::vector<uint8_t> netAccessKey;

  inline bool operator!=(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) != std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
  inline bool operator<(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) < std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
  inline bool operator<=(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) <= std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
  inline bool operator==(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) == std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
  inline bool operator>(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) > std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
  inline bool operator>=(const DppConnectionKeys& rhs) const {
    return std::tie(connector, cSign, netAccessKey) >= std::tie(rhs.connector, rhs.cSign, rhs.netAccessKey);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
