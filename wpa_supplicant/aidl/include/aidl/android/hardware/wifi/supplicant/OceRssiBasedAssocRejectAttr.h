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
class OceRssiBasedAssocRejectAttr {
public:
  int32_t deltaRssi = 0;
  int32_t retryDelayS = 0;

  inline bool operator!=(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) != std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
  inline bool operator<(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) < std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
  inline bool operator<=(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) <= std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
  inline bool operator==(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) == std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
  inline bool operator>(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) > std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
  inline bool operator>=(const OceRssiBasedAssocRejectAttr& rhs) const {
    return std::tie(deltaRssi, retryDelayS) >= std::tie(rhs.deltaRssi, rhs.retryDelayS);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
