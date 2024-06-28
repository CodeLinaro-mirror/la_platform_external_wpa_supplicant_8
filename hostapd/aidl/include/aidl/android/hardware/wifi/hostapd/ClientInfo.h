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

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
class ClientInfo {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  std::string ifaceName;
  std::string apIfaceInstance;
  std::vector<uint8_t> clientAddress;
  bool isConnected = false;

  inline bool operator!=(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) != std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
  inline bool operator<(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) < std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
  inline bool operator<=(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) <= std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
  inline bool operator==(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) == std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
  inline bool operator>(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) > std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
  inline bool operator>=(const ClientInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, clientAddress, isConnected) >= std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.clientAddress, rhs.isConnected);
  }
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
