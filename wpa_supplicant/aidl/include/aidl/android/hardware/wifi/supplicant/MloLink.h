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

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class MloLink {
public:
  int8_t linkId = 0;
  std::vector<uint8_t> staLinkMacAddress;
  int8_t tidsUplinkMap = 0;
  int8_t tidsDownlinkMap = 0;
  std::optional<std::array<uint8_t, 6>> apLinkMacAddress;
  int32_t frequencyMHz = 0;

  inline bool operator!=(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) != std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
  inline bool operator<(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) < std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
  inline bool operator<=(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) <= std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
  inline bool operator==(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) == std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
  inline bool operator>(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) > std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
  inline bool operator>=(const MloLink& rhs) const {
    return std::tie(linkId, staLinkMacAddress, tidsUplinkMap, tidsDownlinkMap, apLinkMacAddress, frequencyMHz) >= std::tie(rhs.linkId, rhs.staLinkMacAddress, rhs.tidsUplinkMap, rhs.tidsDownlinkMap, rhs.apLinkMacAddress, rhs.frequencyMHz);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
