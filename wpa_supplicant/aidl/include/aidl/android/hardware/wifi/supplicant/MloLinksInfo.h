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
#include <aidl/android/hardware/wifi/supplicant/MloLink.h>

namespace aidl::android::hardware::wifi::supplicant {
class MloLink;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class MloLinksInfo {
public:
  std::vector<::aidl::android::hardware::wifi::supplicant::MloLink> links;
  int32_t apMloLinkId = 0;
  std::optional<std::array<uint8_t, 6>> apMldMacAddress;

  inline bool operator!=(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) != std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
  inline bool operator<(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) < std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
  inline bool operator<=(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) <= std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
  inline bool operator==(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) == std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
  inline bool operator>(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) > std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
  inline bool operator>=(const MloLinksInfo& rhs) const {
    return std::tie(links, apMloLinkId, apMldMacAddress) >= std::tie(rhs.links, rhs.apMloLinkId, rhs.apMldMacAddress);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
