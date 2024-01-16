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
#include <aidl/android/hardware/wifi/supplicant/KeyMgmtMask.h>
#include <aidl/android/hardware/wifi/supplicant/StaIfaceCallbackState.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class SupplicantStateChangeData {
public:
  ::aidl::android::hardware::wifi::supplicant::StaIfaceCallbackState newState = ::aidl::android::hardware::wifi::supplicant::StaIfaceCallbackState(0);
  int32_t id = 0;
  std::vector<uint8_t> ssid;
  std::array<uint8_t, 6> bssid = {{}};
  ::aidl::android::hardware::wifi::supplicant::KeyMgmtMask keyMgmtMask = ::aidl::android::hardware::wifi::supplicant::KeyMgmtMask(0);
  int32_t frequencyMhz = 0;
  bool filsHlpSent = false;

  inline bool operator!=(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) != std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
  inline bool operator<(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) < std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
  inline bool operator<=(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) <= std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
  inline bool operator==(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) == std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
  inline bool operator>(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) > std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
  inline bool operator>=(const SupplicantStateChangeData& rhs) const {
    return std::tie(newState, id, ssid, bssid, keyMgmtMask, frequencyMhz, filsHlpSent) >= std::tie(rhs.newState, rhs.id, rhs.ssid, rhs.bssid, rhs.keyMgmtMask, rhs.frequencyMhz, rhs.filsHlpSent);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
