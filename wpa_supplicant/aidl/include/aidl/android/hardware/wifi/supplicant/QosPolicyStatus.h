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
#include <aidl/android/hardware/wifi/supplicant/QosPolicyStatusCode.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class QosPolicyStatus {
public:
  int8_t policyId = 0;
  ::aidl::android::hardware::wifi::supplicant::QosPolicyStatusCode status = ::aidl::android::hardware::wifi::supplicant::QosPolicyStatusCode(0);

  inline bool operator!=(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) != std::tie(rhs.policyId, rhs.status);
  }
  inline bool operator<(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) < std::tie(rhs.policyId, rhs.status);
  }
  inline bool operator<=(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) <= std::tie(rhs.policyId, rhs.status);
  }
  inline bool operator==(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) == std::tie(rhs.policyId, rhs.status);
  }
  inline bool operator>(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) > std::tie(rhs.policyId, rhs.status);
  }
  inline bool operator>=(const QosPolicyStatus& rhs) const {
    return std::tie(policyId, status) >= std::tie(rhs.policyId, rhs.status);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
