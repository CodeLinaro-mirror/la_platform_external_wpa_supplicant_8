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
class FreqRange {
public:
  int32_t min = 0;
  int32_t max = 0;

  inline bool operator!=(const FreqRange& rhs) const {
    return std::tie(min, max) != std::tie(rhs.min, rhs.max);
  }
  inline bool operator<(const FreqRange& rhs) const {
    return std::tie(min, max) < std::tie(rhs.min, rhs.max);
  }
  inline bool operator<=(const FreqRange& rhs) const {
    return std::tie(min, max) <= std::tie(rhs.min, rhs.max);
  }
  inline bool operator==(const FreqRange& rhs) const {
    return std::tie(min, max) == std::tie(rhs.min, rhs.max);
  }
  inline bool operator>(const FreqRange& rhs) const {
    return std::tie(min, max) > std::tie(rhs.min, rhs.max);
  }
  inline bool operator>=(const FreqRange& rhs) const {
    return std::tie(min, max) >= std::tie(rhs.min, rhs.max);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
