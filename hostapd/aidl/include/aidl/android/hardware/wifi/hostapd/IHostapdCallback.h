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
#include <aidl/android/hardware/wifi/hostapd/ApInfo.h>
#include <aidl/android/hardware/wifi/hostapd/ClientInfo.h>

#include <aidl/android/hardware/wifi/hostapd/ICInterface.h>
#include <aidl/android/hardware/wifi/hostapd/ScopedAStatus.h>


namespace aidl::android::hardware::wifi::hostapd {
class ApInfo;
class ClientInfo;
}  // namespace aidl::android::hardware::wifi::hostapd

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {

class IHostapdCallback : public ::ndk::ICInterface {
public:
  virtual ::ndk::ScopedAStatus onApInstanceInfoChanged(const ::aidl::android::hardware::wifi::hostapd::ApInfo& in_apInfo) = 0;
  virtual ::ndk::ScopedAStatus onConnectedClientsChanged(const ::aidl::android::hardware::wifi::hostapd::ClientInfo& in_clientInfo) = 0;
  virtual ::ndk::ScopedAStatus onFailure(const std::string& in_ifaceName, const std::string& in_instanceName) = 0;
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
