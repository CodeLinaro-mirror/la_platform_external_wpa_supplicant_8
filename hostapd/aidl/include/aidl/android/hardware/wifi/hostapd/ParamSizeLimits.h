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
enum class ParamSizeLimits : int32_t {
  SSID_MAX_LEN_IN_BYTES = 32,
  WPA2_PSK_PASSPHRASE_MIN_LEN_IN_BYTES = 8,
  WPA2_PSK_PASSPHRASE_MAX_LEN_IN_BYTES = 63,
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
[[nodiscard]] static inline std::string toString(ParamSizeLimits val) {
  switch(val) {
  case ParamSizeLimits::SSID_MAX_LEN_IN_BYTES:
    return "SSID_MAX_LEN_IN_BYTES";
  case ParamSizeLimits::WPA2_PSK_PASSPHRASE_MIN_LEN_IN_BYTES:
    return "WPA2_PSK_PASSPHRASE_MIN_LEN_IN_BYTES";
  case ParamSizeLimits::WPA2_PSK_PASSPHRASE_MAX_LEN_IN_BYTES:
    return "WPA2_PSK_PASSPHRASE_MAX_LEN_IN_BYTES";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
