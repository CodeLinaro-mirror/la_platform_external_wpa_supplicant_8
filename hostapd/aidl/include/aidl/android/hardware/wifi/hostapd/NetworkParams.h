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
#include <aidl/android/hardware/wifi/hostapd/EncryptionType.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
class NetworkParams {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  std::vector<uint8_t> ssid;
  bool isHidden = false;
  ::aidl::android::hardware::wifi::hostapd::EncryptionType encryptionType = ::aidl::android::hardware::wifi::hostapd::EncryptionType(0);
  std::string passphrase;
  bool isMetered = false;
  std::vector<uint8_t> vendorElements;

  inline bool operator!=(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) != std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
  inline bool operator<(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) < std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
  inline bool operator<=(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) <= std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
  inline bool operator==(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) == std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
  inline bool operator>(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) > std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
  inline bool operator>=(const NetworkParams& rhs) const {
    return std::tie(ssid, isHidden, encryptionType, passphrase, isMetered, vendorElements) >= std::tie(rhs.ssid, rhs.isHidden, rhs.encryptionType, rhs.passphrase, rhs.isMetered, rhs.vendorElements);
  }
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
