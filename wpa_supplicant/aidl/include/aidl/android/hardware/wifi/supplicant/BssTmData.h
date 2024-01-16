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
#include <aidl/android/hardware/wifi/supplicant/BssTmDataFlagsMask.h>
#include <aidl/android/hardware/wifi/supplicant/BssTmStatusCode.h>
#include <aidl/android/hardware/wifi/supplicant/MboCellularDataConnectionPrefValue.h>
#include <aidl/android/hardware/wifi/supplicant/MboTransitionReasonCode.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class BssTmData {
public:
  ::aidl::android::hardware::wifi::supplicant::BssTmStatusCode status = ::aidl::android::hardware::wifi::supplicant::BssTmStatusCode(0);
  ::aidl::android::hardware::wifi::supplicant::BssTmDataFlagsMask flags = ::aidl::android::hardware::wifi::supplicant::BssTmDataFlagsMask(0);
  int32_t assocRetryDelayMs = 0;
  ::aidl::android::hardware::wifi::supplicant::MboTransitionReasonCode mboTransitionReason = ::aidl::android::hardware::wifi::supplicant::MboTransitionReasonCode(0);
  ::aidl::android::hardware::wifi::supplicant::MboCellularDataConnectionPrefValue mboCellPreference = ::aidl::android::hardware::wifi::supplicant::MboCellularDataConnectionPrefValue(0);

  inline bool operator!=(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) != std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
  inline bool operator<(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) < std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
  inline bool operator<=(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) <= std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
  inline bool operator==(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) == std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
  inline bool operator>(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) > std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
  inline bool operator>=(const BssTmData& rhs) const {
    return std::tie(status, flags, assocRetryDelayMs, mboTransitionReason, mboCellPreference) >= std::tie(rhs.status, rhs.flags, rhs.assocRetryDelayMs, rhs.mboTransitionReason, rhs.mboCellPreference);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
