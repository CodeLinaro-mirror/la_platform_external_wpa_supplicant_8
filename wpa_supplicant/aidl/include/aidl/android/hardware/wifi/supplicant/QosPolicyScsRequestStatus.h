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
#include <aidl/android/hardware/wifi/supplicant/QosPolicyScsRequestStatusCode.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class QosPolicyScsRequestStatus {
public:
  int8_t policyId = 0;
  ::aidl::android::hardware::wifi::supplicant::QosPolicyScsRequestStatusCode qosPolicyScsRequestStatusCode = ::aidl::android::hardware::wifi::supplicant::QosPolicyScsRequestStatusCode(0);

  inline bool operator!=(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) != std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
  inline bool operator<(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) < std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
  inline bool operator<=(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) <= std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
  inline bool operator==(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) == std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
  inline bool operator>(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) > std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
  inline bool operator>=(const QosPolicyScsRequestStatus& rhs) const {
    return std::tie(policyId, qosPolicyScsRequestStatusCode) >= std::tie(rhs.policyId, rhs.qosPolicyScsRequestStatusCode);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
