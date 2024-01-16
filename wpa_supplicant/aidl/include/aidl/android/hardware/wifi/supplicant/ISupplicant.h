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
#include <aidl/android/hardware/wifi/supplicant/DebugLevel.h>
#include <aidl/android/hardware/wifi/supplicant/INonStandardCertCallback.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantCallback.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantP2pIface.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantStaIface.h>
#include <aidl/android/hardware/wifi/supplicant/IfaceInfo.h>
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
//class INonStandardCertCallback;
class ISupplicantCallback;
//class ISupplicantP2pIface;
class ISupplicantStaIface;
class IfaceInfo;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicant : public ::ndk::ICInterface {
public:
//  ISupplicant();
//  virtual ~ISupplicant();

  enum : int32_t { EXT_RADIO_WORK_TIMEOUT_IN_SECS = 10 };
  static const int32_t version = 2;

  static bool setDefaultImpl(const std::shared_ptr<ISupplicant>& impl);
  static const std::shared_ptr<ISupplicant>& getDefaultImpl();
//  virtual ::ndk::ScopedAStatus addP2pInterface(const std::string& in_ifName, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantP2pIface>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addStaInterface(const std::string& in_ifName, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaIface>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getDebugLevel(::aidl::android::hardware::wifi::supplicant::DebugLevel* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getP2pInterface(const std::string& in_ifName, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantP2pIface>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getStaInterface(const std::string& in_ifName, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaIface>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus isDebugShowKeysEnabled(bool* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus isDebugShowTimestampEnabled(bool* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus listInterfaces(std::vector<::aidl::android::hardware::wifi::supplicant::IfaceInfo>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus registerCallback(const std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantCallback>& in_callback) = 0;
  virtual ::ndk::ScopedAStatus removeInterface(const ::aidl::android::hardware::wifi::supplicant::IfaceInfo& in_ifaceInfo) = 0;
  virtual ::ndk::ScopedAStatus setConcurrencyPriority(::aidl::android::hardware::wifi::supplicant::IfaceType in_type) = 0;
  virtual ::ndk::ScopedAStatus setDebugParams(::aidl::android::hardware::wifi::supplicant::DebugLevel in_level, bool in_showTimestamp, bool in_showKeys) = 0;
  virtual ::ndk::ScopedAStatus terminate() = 0;
//  virtual ::ndk::ScopedAStatus registerNonStandardCertCallback(const std::shared_ptr<::aidl::android::hardware::wifi::supplicant::INonStandardCertCallback>& in_callback) = 0;
private:
  static std::shared_ptr<ISupplicant> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
