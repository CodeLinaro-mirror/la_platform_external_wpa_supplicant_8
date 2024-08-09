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
#include <aidl/vendor/qti/hardware/wifi/hostapd/IHostapdVendorCallback.h>
#include <aidl/android/hardware/wifi/hostapd/Ieee80211ReasonCode.h>
#include <aidl/android/hardware/wifi/hostapd/ICInterface.h>
#include <aidl/android/hardware/wifi/hostapd/ScopedAStatus.h>

namespace aidl::vendor::qti::hardware::wifi::hostapd {
class IHostapdVendorCallback;
}  // namespace aidl::vendor::qti::hardware::wifi::hostapd
namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace hostapd {

class IHostapdVendor : public ::ndk::ICInterface {
public:
  //IHostapdVendor();
  //virtual ~IHostapdVendor();
  virtual ::ndk::ScopedAStatus listVendorInterfaces(std::vector<std::string>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus doDriverCmd(const std::string& in_iface, const std::string& in_cmd, std::string* _aidl_return) = 0;
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
