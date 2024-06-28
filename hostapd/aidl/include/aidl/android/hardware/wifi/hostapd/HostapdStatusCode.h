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
enum class HostapdStatusCode : int32_t {
  SUCCESS = 0,
  FAILURE_UNKNOWN = 1,
  FAILURE_ARGS_INVALID = 2,
  FAILURE_IFACE_UNKNOWN = 3,
  FAILURE_IFACE_EXISTS = 4,
  FAILURE_CLIENT_UNKNOWN = 5,
};
  
class HostapdStatus {
public:
  HostapdStatus() :
    code(HostapdStatusCode::SUCCESS), description("") {}
	
  HostapdStatus(const HostapdStatusCode &_code) :
    code(_code), description("") {}
	
  HostapdStatus(const HostapdStatusCode &_code, const std::string &_description) :
    code(_code), description(_description) {}
	
  inline bool operator==(const HostapdStatus& other) const {
    return (code == other.code && description == other.description);
  }
  inline bool operator!=(const HostapdStatus &other) const {
    return !((*this) == other);
  }
  HostapdStatusCode code;
  std::string description;
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
[[nodiscard]] static inline std::string toString(HostapdStatusCode val) {
  switch(val) {
  case HostapdStatusCode::SUCCESS:
    return "SUCCESS";
  case HostapdStatusCode::FAILURE_UNKNOWN:
    return "FAILURE_UNKNOWN";
  case HostapdStatusCode::FAILURE_ARGS_INVALID:
    return "FAILURE_ARGS_INVALID";
  case HostapdStatusCode::FAILURE_IFACE_UNKNOWN:
    return "FAILURE_IFACE_UNKNOWN";
  case HostapdStatusCode::FAILURE_IFACE_EXISTS:
    return "FAILURE_IFACE_EXISTS";
  case HostapdStatusCode::FAILURE_CLIENT_UNKNOWN:
    return "FAILURE_CLIENT_UNKNOWN";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
