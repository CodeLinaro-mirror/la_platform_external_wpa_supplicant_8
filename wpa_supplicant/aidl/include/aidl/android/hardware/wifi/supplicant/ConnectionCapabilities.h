/*
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
#include <aidl/android/hardware/wifi/supplicant/LegacyMode.h>
#include <aidl/android/hardware/wifi/supplicant/WifiTechnology.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ConnectionCapabilities {
public:
  ::aidl::android::hardware::wifi::supplicant::WifiTechnology technology = ::aidl::android::hardware::wifi::supplicant::WifiTechnology(0);
  int32_t channelBandwidth = 0;
  int32_t maxNumberTxSpatialStreams = 0;
  int32_t maxNumberRxSpatialStreams = 0;
  ::aidl::android::hardware::wifi::supplicant::LegacyMode legacyMode = ::aidl::android::hardware::wifi::supplicant::LegacyMode(0);
  bool apTidToLinkMapNegotiationSupported = false;

  inline bool operator!=(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) != std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
  inline bool operator<(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) < std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
  inline bool operator<=(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) <= std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
  inline bool operator==(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) == std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
  inline bool operator>(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) > std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
  inline bool operator>=(const ConnectionCapabilities& rhs) const {
    return std::tie(technology, channelBandwidth, maxNumberTxSpatialStreams, maxNumberRxSpatialStreams, legacyMode, apTidToLinkMapNegotiationSupported) >= std::tie(rhs.technology, rhs.channelBandwidth, rhs.maxNumberTxSpatialStreams, rhs.maxNumberRxSpatialStreams, rhs.legacyMode, rhs.apTidToLinkMapNegotiationSupported);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
