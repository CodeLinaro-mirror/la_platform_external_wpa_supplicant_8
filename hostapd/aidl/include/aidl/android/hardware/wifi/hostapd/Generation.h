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
enum class Generation : int32_t {
  WIFI_STANDARD_UNKNOWN = -1,
  WIFI_STANDARD_LEGACY = 0,
  WIFI_STANDARD_11N = 1,
  WIFI_STANDARD_11AC = 2,
  WIFI_STANDARD_11AD = 3,
  WIFI_STANDARD_11AX = 4,
  WIFI_STANDARD_11BE = 5,
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
[[nodiscard]] static inline std::string toString(Generation val) {
  switch(val) {
  case Generation::WIFI_STANDARD_UNKNOWN:
    return "WIFI_STANDARD_UNKNOWN";
  case Generation::WIFI_STANDARD_LEGACY:
    return "WIFI_STANDARD_LEGACY";
  case Generation::WIFI_STANDARD_11N:
    return "WIFI_STANDARD_11N";
  case Generation::WIFI_STANDARD_11AC:
    return "WIFI_STANDARD_11AC";
  case Generation::WIFI_STANDARD_11AD:
    return "WIFI_STANDARD_11AD";
  case Generation::WIFI_STANDARD_11AX:
    return "WIFI_STANDARD_11AX";
  case Generation::WIFI_STANDARD_11BE:
    return "WIFI_STANDARD_11BE";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
