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

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
enum class ChannelBandwidth : int32_t {
  BANDWIDTH_INVALID = 0,
  BANDWIDTH_AUTO = 1,
  BANDWIDTH_20_NOHT = 2,
  BANDWIDTH_20 = 3,
  BANDWIDTH_40 = 4,
  BANDWIDTH_80 = 5,
  BANDWIDTH_80P80 = 6,
  BANDWIDTH_160 = 7,
  BANDWIDTH_320 = 8,
  BANDWIDTH_2160 = 9,
  BANDWIDTH_4320 = 10,
  BANDWIDTH_6480 = 11,
  BANDWIDTH_8640 = 12,
};

}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
[[nodiscard]] static inline std::string toString(ChannelBandwidth val) {
  switch(val) {
  case ChannelBandwidth::BANDWIDTH_INVALID:
    return "BANDWIDTH_INVALID";
  case ChannelBandwidth::BANDWIDTH_AUTO:
    return "BANDWIDTH_AUTO";
  case ChannelBandwidth::BANDWIDTH_20_NOHT:
    return "BANDWIDTH_20_NOHT";
  case ChannelBandwidth::BANDWIDTH_20:
    return "BANDWIDTH_20";
  case ChannelBandwidth::BANDWIDTH_40:
    return "BANDWIDTH_40";
  case ChannelBandwidth::BANDWIDTH_80:
    return "BANDWIDTH_80";
  case ChannelBandwidth::BANDWIDTH_80P80:
    return "BANDWIDTH_80P80";
  case ChannelBandwidth::BANDWIDTH_160:
    return "BANDWIDTH_160";
  case ChannelBandwidth::BANDWIDTH_320:
    return "BANDWIDTH_320";
  case ChannelBandwidth::BANDWIDTH_2160:
    return "BANDWIDTH_2160";
  case ChannelBandwidth::BANDWIDTH_4320:
    return "BANDWIDTH_4320";
  case ChannelBandwidth::BANDWIDTH_6480:
    return "BANDWIDTH_6480";
  case ChannelBandwidth::BANDWIDTH_8640:
    return "BANDWIDTH_8640";
  default:
    return std::to_string(static_cast<int32_t>(val));
	}
}
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
