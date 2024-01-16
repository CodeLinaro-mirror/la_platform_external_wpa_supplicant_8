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
class NetworkResponseEapSimUmtsAuthParams {
public:
  std::vector<uint8_t> res;
  std::vector<uint8_t> ik;
  std::vector<uint8_t> ck;

  inline bool operator!=(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) != std::tie(rhs.res, rhs.ik, rhs.ck);
  }
  inline bool operator<(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) < std::tie(rhs.res, rhs.ik, rhs.ck);
  }
  inline bool operator<=(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) <= std::tie(rhs.res, rhs.ik, rhs.ck);
  }
  inline bool operator==(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) == std::tie(rhs.res, rhs.ik, rhs.ck);
  }
  inline bool operator>(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) > std::tie(rhs.res, rhs.ik, rhs.ck);
  }
  inline bool operator>=(const NetworkResponseEapSimUmtsAuthParams& rhs) const {
    return std::tie(res, ik, ck) >= std::tie(rhs.res, rhs.ik, rhs.ck);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
