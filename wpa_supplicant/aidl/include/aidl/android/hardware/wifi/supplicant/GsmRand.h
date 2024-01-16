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
class GsmRand {
public:
  std::vector<uint8_t> data;

  inline bool operator!=(const GsmRand& rhs) const {
    return std::tie(data) != std::tie(rhs.data);
  }
  inline bool operator<(const GsmRand& rhs) const {
    return std::tie(data) < std::tie(rhs.data);
  }
  inline bool operator<=(const GsmRand& rhs) const {
    return std::tie(data) <= std::tie(rhs.data);
  }
  inline bool operator==(const GsmRand& rhs) const {
    return std::tie(data) == std::tie(rhs.data);
  }
  inline bool operator>(const GsmRand& rhs) const {
    return std::tie(data) > std::tie(rhs.data);
  }
  inline bool operator>=(const GsmRand& rhs) const {
    return std::tie(data) >= std::tie(rhs.data);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
