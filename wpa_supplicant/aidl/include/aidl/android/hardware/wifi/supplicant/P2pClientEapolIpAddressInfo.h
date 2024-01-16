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
class P2pClientEapolIpAddressInfo {
public:
  int32_t ipAddressClient = 0;
  int32_t ipAddressMask = 0;
  int32_t ipAddressGo = 0;

  inline bool operator!=(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) != std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
  inline bool operator<(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) < std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
  inline bool operator<=(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) <= std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
  inline bool operator==(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) == std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
  inline bool operator>(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) > std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
  inline bool operator>=(const P2pClientEapolIpAddressInfo& rhs) const {
    return std::tie(ipAddressClient, ipAddressMask, ipAddressGo) >= std::tie(rhs.ipAddressClient, rhs.ipAddressMask, rhs.ipAddressGo);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
