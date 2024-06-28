/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

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
#include <aidl/android/hardware/wifi/hostapd/ChannelBandwidth.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
class HwModeParams {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  bool enable80211N = false;
  bool enable80211AC = false;
  bool enable80211AX = false;
  bool enable6GhzBand = false;
  bool enableHeSingleUserBeamformer = false;
  bool enableHeSingleUserBeamformee = false;
  bool enableHeMultiUserBeamformer = false;
  bool enableHeTargetWakeTime = false;
  bool enableEdmg = false;
  bool enable80211BE = false;
  ::aidl::android::hardware::wifi::hostapd::ChannelBandwidth maximumChannelBandwidth = ::aidl::android::hardware::wifi::hostapd::ChannelBandwidth(0);

  inline bool operator!=(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) != std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
  inline bool operator<(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) < std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
  inline bool operator<=(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) <= std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
  inline bool operator==(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) == std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
  inline bool operator>(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) > std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
  inline bool operator>=(const HwModeParams& rhs) const {
    return std::tie(enable80211N, enable80211AC, enable80211AX, enable6GhzBand, enableHeSingleUserBeamformer, enableHeSingleUserBeamformee, enableHeMultiUserBeamformer, enableHeTargetWakeTime, enableEdmg, enable80211BE, maximumChannelBandwidth) >= std::tie(rhs.enable80211N, rhs.enable80211AC, rhs.enable80211AX, rhs.enable6GhzBand, rhs.enableHeSingleUserBeamformer, rhs.enableHeSingleUserBeamformee, rhs.enableHeMultiUserBeamformer, rhs.enableHeTargetWakeTime, rhs.enableEdmg, rhs.enable80211BE, rhs.maximumChannelBandwidth);
  }
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
