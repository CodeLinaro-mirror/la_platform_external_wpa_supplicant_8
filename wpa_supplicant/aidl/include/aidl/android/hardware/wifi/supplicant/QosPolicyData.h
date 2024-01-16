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
#include <aidl/android/hardware/wifi/supplicant/QosPolicyClassifierParams.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyRequestType.h>

namespace aidl::android::hardware::wifi::supplicant {
class QosPolicyClassifierParams;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class QosPolicyData {
public:
  int8_t policyId = 0;
  ::aidl::android::hardware::wifi::supplicant::QosPolicyRequestType requestType = ::aidl::android::hardware::wifi::supplicant::QosPolicyRequestType(0);
  int8_t dscp = 0;
  ::aidl::android::hardware::wifi::supplicant::QosPolicyClassifierParams classifierParams;

  inline bool operator!=(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) != std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
  inline bool operator<(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) < std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
  inline bool operator<=(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) <= std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
  inline bool operator==(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) == std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
  inline bool operator>(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) > std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
  inline bool operator>=(const QosPolicyData& rhs) const {
    return std::tie(policyId, requestType, dscp, classifierParams) >= std::tie(rhs.policyId, rhs.requestType, rhs.dscp, rhs.classifierParams);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
