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
class NetworkRequestEapSimUmtsAuthParams {
public:
  std::vector<uint8_t> rand;
  std::vector<uint8_t> autn;

  inline bool operator!=(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) != std::tie(rhs.rand, rhs.autn);
  }
  inline bool operator<(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) < std::tie(rhs.rand, rhs.autn);
  }
  inline bool operator<=(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) <= std::tie(rhs.rand, rhs.autn);
  }
  inline bool operator==(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) == std::tie(rhs.rand, rhs.autn);
  }
  inline bool operator>(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) > std::tie(rhs.rand, rhs.autn);
  }
  inline bool operator>=(const NetworkRequestEapSimUmtsAuthParams& rhs) const {
    return std::tie(rand, autn) >= std::tie(rhs.rand, rhs.autn);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
