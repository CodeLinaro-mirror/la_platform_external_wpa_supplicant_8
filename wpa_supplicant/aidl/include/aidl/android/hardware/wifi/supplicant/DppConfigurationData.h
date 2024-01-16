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
#include <aidl/android/hardware/wifi/supplicant/DppAkm.h>
#include <aidl/android/hardware/wifi/supplicant/DppConnectionKeys.h>

namespace aidl::android::hardware::wifi::supplicant {
class DppConnectionKeys;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class DppConfigurationData {
public:
  std::vector<uint8_t> ssid;
  std::string password;
  std::vector<uint8_t> psk;
  ::aidl::android::hardware::wifi::supplicant::DppAkm securityAkm = ::aidl::android::hardware::wifi::supplicant::DppAkm(0);
  ::aidl::android::hardware::wifi::supplicant::DppConnectionKeys dppConnectionKeys;
  bool connStatusRequested = false;

  inline bool operator!=(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) != std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
  inline bool operator<(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) < std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
  inline bool operator<=(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) <= std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
  inline bool operator==(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) == std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
  inline bool operator>(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) > std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
  inline bool operator>=(const DppConfigurationData& rhs) const {
    return std::tie(ssid, password, psk, securityAkm, dppConnectionKeys, connStatusRequested) >= std::tie(rhs.ssid, rhs.password, rhs.psk, rhs.securityAkm, rhs.dppConnectionKeys, rhs.connStatusRequested);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
