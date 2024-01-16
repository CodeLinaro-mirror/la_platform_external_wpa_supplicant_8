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
#include <aidl/android/hardware/wifi/supplicant/QosPolicyScsResponseStatusCode.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class QosPolicyScsResponseStatus {
public:
  int8_t policyId = 0;
  ::aidl::android::hardware::wifi::supplicant::QosPolicyScsResponseStatusCode qosPolicyScsResponseStatusCode = ::aidl::android::hardware::wifi::supplicant::QosPolicyScsResponseStatusCode(0);

  inline bool operator!=(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) != std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
  inline bool operator<(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) < std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
  inline bool operator<=(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) <= std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
  inline bool operator==(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) == std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
  inline bool operator>(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) > std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
  inline bool operator>=(const QosPolicyScsResponseStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsResponseStatusCode) >= std::tie(rhs.policyId, rhs.qosPolicyScsResponseStatusCode);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
