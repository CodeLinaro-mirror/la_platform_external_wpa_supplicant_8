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
#include <aidl/android/hardware/wifi/hostapd/Generation.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
class ApInfo {
public:
  typedef std::false_type fixed_size;
  static const char* descriptor;

  std::string ifaceName;
  std::string apIfaceInstance;
  int32_t freqMhz = 0;
  ::aidl::android::hardware::wifi::hostapd::ChannelBandwidth channelBandwidth = ::aidl::android::hardware::wifi::hostapd::ChannelBandwidth(0);
  ::aidl::android::hardware::wifi::hostapd::Generation generation = ::aidl::android::hardware::wifi::hostapd::Generation(0);
  std::vector<uint8_t> apIfaceInstanceMacAddress;


  inline bool operator!=(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) != std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }
  inline bool operator<(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) < std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }
  inline bool operator<=(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) <= std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }
  inline bool operator==(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) == std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }
  inline bool operator>(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) > std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }
  inline bool operator>=(const ApInfo& rhs) const {
    return std::tie(ifaceName, apIfaceInstance, freqMhz, channelBandwidth, generation, apIfaceInstanceMacAddress) >= std::tie(rhs.ifaceName, rhs.apIfaceInstance, rhs.freqMhz, rhs.channelBandwidth, rhs.generation, rhs.apIfaceInstanceMacAddress);
  }

};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
