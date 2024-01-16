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
#include <aidl/android/hardware/wifi/supplicant/P2pGroupCapabilityMask.h>
#include <aidl/android/hardware/wifi/supplicant/P2pGroupStartedEventParams.h>
#include <aidl/android/hardware/wifi/supplicant/P2pProvDiscStatusCode.h>
#include <aidl/android/hardware/wifi/supplicant/P2pStatusCode.h>
#include <aidl/android/hardware/wifi/supplicant/WpsConfigMethods.h>
#include <aidl/android/hardware/wifi/supplicant/WpsDevPasswordId.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class P2pGroupStartedEventParams;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantP2pIfaceCallback : public ::ndk::ICInterface {
public:
  static bool setDefaultImpl(const std::shared_ptr<ISupplicantP2pIfaceCallback>& impl);
  static const std::shared_ptr<ISupplicantP2pIfaceCallback>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus onDeviceFound(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_p2pDeviceAddress, const std::vector<uint8_t>& in_primaryDeviceType, const std::string& in_deviceName, ::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods, int8_t in_deviceCapabilities, ::aidl::android::hardware::wifi::supplicant::P2pGroupCapabilityMask in_groupCapabilities, const std::vector<uint8_t>& in_wfdDeviceInfo) = 0;
  virtual ::ndk::ScopedAStatus onDeviceLost(const std::vector<uint8_t>& in_p2pDeviceAddress) = 0;
  virtual ::ndk::ScopedAStatus onFindStopped() = 0;
  virtual ::ndk::ScopedAStatus onGoNegotiationCompleted(::aidl::android::hardware::wifi::supplicant::P2pStatusCode in_status) = 0;
  virtual ::ndk::ScopedAStatus onGoNegotiationRequest(const std::vector<uint8_t>& in_srcAddress, ::aidl::android::hardware::wifi::supplicant::WpsDevPasswordId in_passwordId) = 0;
  virtual ::ndk::ScopedAStatus onGroupFormationFailure(const std::string& in_failureReason) = 0;
  virtual ::ndk::ScopedAStatus onGroupFormationSuccess() = 0;
  virtual ::ndk::ScopedAStatus onGroupRemoved(const std::string& in_groupIfname, bool in_isGroupOwner) = 0;
  virtual ::ndk::ScopedAStatus onGroupStarted(const std::string& in_groupIfname, bool in_isGroupOwner, const std::vector<uint8_t>& in_ssid, int32_t in_frequency, const std::vector<uint8_t>& in_psk, const std::string& in_passphrase, const std::vector<uint8_t>& in_goDeviceAddress, bool in_isPersistent) = 0;
  virtual ::ndk::ScopedAStatus onInvitationReceived(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_goDeviceAddress, const std::vector<uint8_t>& in_bssid, int32_t in_persistentNetworkId, int32_t in_operatingFrequency) = 0;
  virtual ::ndk::ScopedAStatus onInvitationResult(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::P2pStatusCode in_status) = 0;
  virtual ::ndk::ScopedAStatus onProvisionDiscoveryCompleted(const std::vector<uint8_t>& in_p2pDeviceAddress, bool in_isRequest, ::aidl::android::hardware::wifi::supplicant::P2pProvDiscStatusCode in_status, ::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods, const std::string& in_generatedPin) = 0;
  virtual ::ndk::ScopedAStatus onR2DeviceFound(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_p2pDeviceAddress, const std::vector<uint8_t>& in_primaryDeviceType, const std::string& in_deviceName, ::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods, int8_t in_deviceCapabilities, ::aidl::android::hardware::wifi::supplicant::P2pGroupCapabilityMask in_groupCapabilities, const std::vector<uint8_t>& in_wfdDeviceInfo, const std::vector<uint8_t>& in_wfdR2DeviceInfo) = 0;
  virtual ::ndk::ScopedAStatus onServiceDiscoveryResponse(const std::vector<uint8_t>& in_srcAddress, char16_t in_updateIndicator, const std::vector<uint8_t>& in_tlvs) = 0;
  virtual ::ndk::ScopedAStatus onStaAuthorized(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_p2pDeviceAddress) = 0;
  virtual ::ndk::ScopedAStatus onStaDeauthorized(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_p2pDeviceAddress) = 0;
  virtual ::ndk::ScopedAStatus onGroupFrequencyChanged(const std::string& in_groupIfname, int32_t in_frequency) = 0;
  virtual ::ndk::ScopedAStatus onDeviceFoundWithVendorElements(const std::vector<uint8_t>& in_srcAddress, const std::vector<uint8_t>& in_p2pDeviceAddress, const std::vector<uint8_t>& in_primaryDeviceType, const std::string& in_deviceName, ::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods, int8_t in_deviceCapabilities, ::aidl::android::hardware::wifi::supplicant::P2pGroupCapabilityMask in_groupCapabilities, const std::vector<uint8_t>& in_wfdDeviceInfo, const std::vector<uint8_t>& in_wfdR2DeviceInfo, const std::vector<uint8_t>& in_vendorElemBytes) = 0;
  virtual ::ndk::ScopedAStatus onGroupStartedWithParams(const ::aidl::android::hardware::wifi::supplicant::P2pGroupStartedEventParams& in_groupStartedEventParams) = 0;
private:
  static std::shared_ptr<ISupplicantP2pIfaceCallback> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
