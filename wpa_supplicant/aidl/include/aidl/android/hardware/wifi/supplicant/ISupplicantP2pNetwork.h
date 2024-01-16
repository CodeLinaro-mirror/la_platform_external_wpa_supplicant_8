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
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>
#include <aidl/android/hardware/wifi/supplicant/MacAddress.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class MacAddress;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantP2pNetwork : public ::ndk::ICInterface {
public:
  static bool setDefaultImpl(const std::shared_ptr<ISupplicantP2pNetwork>& impl);
  static const std::shared_ptr<ISupplicantP2pNetwork>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus getBssid(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getClientList(std::vector<::aidl::android::hardware::wifi::supplicant::MacAddress>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getId(int32_t* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getInterfaceName(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getSsid(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getType(::aidl::android::hardware::wifi::supplicant::IfaceType* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus isCurrent(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus isGroupOwner(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus isPersistent(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus setClientList(const std::vector<::aidl::android::hardware::wifi::supplicant::MacAddress>& in_clients) = 0;
private:
  static std::shared_ptr<ISupplicantP2pNetwork> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
