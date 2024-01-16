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
#include <aidl/android/hardware/wifi/supplicant/MboAssocDisallowedReasonCode.h>
#include <aidl/android/hardware/wifi/supplicant/OceRssiBasedAssocRejectAttr.h>
#include <aidl/android/hardware/wifi/supplicant/StaIfaceStatusCode.h>

namespace aidl::android::hardware::wifi::supplicant {
class OceRssiBasedAssocRejectAttr;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class AssociationRejectionData {
public:
  std::vector<uint8_t> ssid;
  std::vector<uint8_t> bssid;
  ::aidl::android::hardware::wifi::supplicant::StaIfaceStatusCode statusCode = ::aidl::android::hardware::wifi::supplicant::StaIfaceStatusCode(0);
  bool timedOut = false;
  bool isMboAssocDisallowedReasonCodePresent = false;
  ::aidl::android::hardware::wifi::supplicant::MboAssocDisallowedReasonCode mboAssocDisallowedReason = ::aidl::android::hardware::wifi::supplicant::MboAssocDisallowedReasonCode(0);
  bool isOceRssiBasedAssocRejectAttrPresent = false;
  ::aidl::android::hardware::wifi::supplicant::OceRssiBasedAssocRejectAttr oceRssiBasedAssocRejectData;

  inline bool operator!=(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) != std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
  inline bool operator<(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) < std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
  inline bool operator<=(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) <= std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
  inline bool operator==(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) == std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
  inline bool operator>(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) > std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
  inline bool operator>=(const AssociationRejectionData& rhs) const {
    return std::tie(ssid, bssid, statusCode, timedOut, isMboAssocDisallowedReasonCodePresent, mboAssocDisallowedReason, isOceRssiBasedAssocRejectAttrPresent, oceRssiBasedAssocRejectData) >= std::tie(rhs.ssid, rhs.bssid, rhs.statusCode, rhs.timedOut, rhs.isMboAssocDisallowedReasonCodePresent, rhs.mboAssocDisallowedReason, rhs.isOceRssiBasedAssocRejectAttrPresent, rhs.oceRssiBasedAssocRejectData);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
