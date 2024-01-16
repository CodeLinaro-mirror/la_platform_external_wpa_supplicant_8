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
#include <aidl/android/hardware/wifi/supplicant/P2pClientEapolIpAddressInfo.h>

namespace aidl::android::hardware::wifi::supplicant {
class P2pClientEapolIpAddressInfo;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class P2pGroupStartedEventParams {
public:
  std::string groupInterfaceName;
  bool isGroupOwner = false;
  std::vector<uint8_t> ssid;
  int32_t frequencyMHz = 0;
  std::vector<uint8_t> psk;
  std::string passphrase;
  bool isPersistent = false;
  std::array<uint8_t, 6> goDeviceAddress = {{}};
  std::array<uint8_t, 6> goInterfaceAddress = {{}};
  bool isP2pClientEapolIpAddressInfoPresent = false;
  ::aidl::android::hardware::wifi::supplicant::P2pClientEapolIpAddressInfo p2pClientIpInfo;

  inline bool operator!=(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) != std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
  inline bool operator<(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) < std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
  inline bool operator<=(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) <= std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
  inline bool operator==(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) == std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
  inline bool operator>(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) > std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
  inline bool operator>=(const P2pGroupStartedEventParams& rhs) const {
    return std::tie(groupInterfaceName, isGroupOwner, ssid, frequencyMHz, psk, passphrase, isPersistent, goDeviceAddress, goInterfaceAddress, isP2pClientEapolIpAddressInfoPresent, p2pClientIpInfo) >= std::tie(rhs.groupInterfaceName, rhs.isGroupOwner, rhs.ssid, rhs.frequencyMHz, rhs.psk, rhs.passphrase, rhs.isPersistent, rhs.goDeviceAddress, rhs.goInterfaceAddress, rhs.isP2pClientEapolIpAddressInfoPresent, rhs.p2pClientIpInfo);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
