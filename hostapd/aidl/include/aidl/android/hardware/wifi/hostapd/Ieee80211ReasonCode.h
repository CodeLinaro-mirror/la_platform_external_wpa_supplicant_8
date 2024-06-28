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
enum class Ieee80211ReasonCode : int32_t {
  WLAN_REASON_UNSPECIFIED = 1,
  WLAN_REASON_PREV_AUTH_NOT_VALID = 2,
  WLAN_REASON_DISASSOC_AP_BUSY = 5,
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
[[nodiscard]] static inline std::string toString(Ieee80211ReasonCode val) {
  switch(val) {
  case Ieee80211ReasonCode::WLAN_REASON_UNSPECIFIED:
    return "WLAN_REASON_UNSPECIFIED";
  case Ieee80211ReasonCode::WLAN_REASON_PREV_AUTH_NOT_VALID:
    return "WLAN_REASON_PREV_AUTH_NOT_VALID";
  case Ieee80211ReasonCode::WLAN_REASON_DISASSOC_AP_BUSY:
    return "WLAN_REASON_DISASSOC_AP_BUSY";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
