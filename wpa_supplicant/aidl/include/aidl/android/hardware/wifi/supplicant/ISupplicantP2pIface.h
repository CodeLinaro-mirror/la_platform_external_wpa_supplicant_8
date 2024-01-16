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
#include <aidl/android/hardware/wifi/supplicant/FreqRange.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantP2pIfaceCallback.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantP2pNetwork.h>
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>
#include <aidl/android/hardware/wifi/supplicant/MiracastMode.h>
#include <aidl/android/hardware/wifi/supplicant/P2pFrameTypeMask.h>
#include <aidl/android/hardware/wifi/supplicant/P2pGroupCapabilityMask.h>
#include <aidl/android/hardware/wifi/supplicant/WpsConfigMethods.h>
#include <aidl/android/hardware/wifi/supplicant/WpsProvisionMethod.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class FreqRange;
class ISupplicantP2pIfaceCallback;
class ISupplicantP2pNetwork;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantP2pIface : public ::ndk::ICInterface {
public:
  static bool setDefaultImpl(const std::shared_ptr<ISupplicantP2pIface>& impl);
  static const std::shared_ptr<ISupplicantP2pIface>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus addBonjourService(const std::vector<uint8_t>& in_query, const std::vector<uint8_t>& in_response) = 0;
  virtual ::ndk::ScopedAStatus addGroup(bool in_persistent, int32_t in_persistentNetworkId) = 0;
  virtual ::ndk::ScopedAStatus addGroupWithConfig(const std::vector<uint8_t>& in_ssid, const std::string& in_pskPassphrase, bool in_persistent, int32_t in_freq, const std::vector<uint8_t>& in_peerAddress, bool in_joinExistingGroup) = 0;
  virtual ::ndk::ScopedAStatus addNetwork(std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantP2pNetwork>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addUpnpService(int32_t in_version, const std::string& in_serviceName) = 0;
  virtual ::ndk::ScopedAStatus cancelConnect() = 0;
  virtual ::ndk::ScopedAStatus cancelServiceDiscovery(int64_t in_identifier) = 0;
  virtual ::ndk::ScopedAStatus cancelWps(const std::string& in_groupIfName) = 0;
  virtual ::ndk::ScopedAStatus configureExtListen(int32_t in_periodInMillis, int32_t in_intervalInMillis) = 0;
  virtual ::ndk::ScopedAStatus connect(const std::vector<uint8_t>& in_peerAddress, ::aidl::android::hardware::wifi::supplicant::WpsProvisionMethod in_provisionMethod, const std::string& in_preSelectedPin, bool in_joinExistingGroup, bool in_persistent, int32_t in_goIntent, std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus createNfcHandoverRequestMessage(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus createNfcHandoverSelectMessage(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus enableWfd(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus find(int32_t in_timeoutInSec) = 0;
  virtual ::ndk::ScopedAStatus flush() = 0;
  virtual ::ndk::ScopedAStatus flushServices() = 0;
  virtual ::ndk::ScopedAStatus getDeviceAddress(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getEdmg(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getGroupCapability(const std::vector<uint8_t>& in_peerAddress, ::aidl::android::hardware::wifi::supplicant::P2pGroupCapabilityMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getName(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getNetwork(int32_t in_id, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantP2pNetwork>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getSsid(const std::vector<uint8_t>& in_peerAddress, std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getType(::aidl::android::hardware::wifi::supplicant::IfaceType* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus invite(const std::string& in_groupIfName, const std::vector<uint8_t>& in_goDeviceAddress, const std::vector<uint8_t>& in_peerAddress) = 0;
  virtual ::ndk::ScopedAStatus listNetworks(std::vector<int32_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus provisionDiscovery(const std::vector<uint8_t>& in_peerAddress, ::aidl::android::hardware::wifi::supplicant::WpsProvisionMethod in_provisionMethod) = 0;
  virtual ::ndk::ScopedAStatus registerCallback(const std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantP2pIfaceCallback>& in_callback) = 0;
  virtual ::ndk::ScopedAStatus reinvoke(int32_t in_persistentNetworkId, const std::vector<uint8_t>& in_peerAddress) = 0;
  virtual ::ndk::ScopedAStatus reject(const std::vector<uint8_t>& in_peerAddress) = 0;
  virtual ::ndk::ScopedAStatus removeBonjourService(const std::vector<uint8_t>& in_query) = 0;
  virtual ::ndk::ScopedAStatus removeGroup(const std::string& in_groupIfName) = 0;
  virtual ::ndk::ScopedAStatus removeNetwork(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus removeUpnpService(int32_t in_version, const std::string& in_serviceName) = 0;
  virtual ::ndk::ScopedAStatus reportNfcHandoverInitiation(const std::vector<uint8_t>& in_select) = 0;
  virtual ::ndk::ScopedAStatus reportNfcHandoverResponse(const std::vector<uint8_t>& in_request) = 0;
  virtual ::ndk::ScopedAStatus requestServiceDiscovery(const std::vector<uint8_t>& in_peerAddress, const std::vector<uint8_t>& in_query, int64_t* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus saveConfig() = 0;
  virtual ::ndk::ScopedAStatus setDisallowedFrequencies(const std::vector<::aidl::android::hardware::wifi::supplicant::FreqRange>& in_ranges) = 0;
  virtual ::ndk::ScopedAStatus setEdmg(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setGroupIdle(const std::string& in_groupIfName, int32_t in_timeoutInSec) = 0;
  virtual ::ndk::ScopedAStatus setListenChannel(int32_t in_channel, int32_t in_operatingClass) = 0;
  virtual ::ndk::ScopedAStatus setMacRandomization(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setMiracastMode(::aidl::android::hardware::wifi::supplicant::MiracastMode in_mode) = 0;
  virtual ::ndk::ScopedAStatus setPowerSave(const std::string& in_groupIfName, bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setSsidPostfix(const std::vector<uint8_t>& in_postfix) = 0;
  virtual ::ndk::ScopedAStatus setWfdDeviceInfo(const std::vector<uint8_t>& in_info) = 0;
  virtual ::ndk::ScopedAStatus setWfdR2DeviceInfo(const std::vector<uint8_t>& in_info) = 0;
  virtual ::ndk::ScopedAStatus removeClient(const std::vector<uint8_t>& in_peerAddress, bool in_isLegacyClient) = 0;
  virtual ::ndk::ScopedAStatus setWpsConfigMethods(::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods) = 0;
  virtual ::ndk::ScopedAStatus setWpsDeviceName(const std::string& in_name) = 0;
  virtual ::ndk::ScopedAStatus setWpsDeviceType(const std::vector<uint8_t>& in_type) = 0;
  virtual ::ndk::ScopedAStatus setWpsManufacturer(const std::string& in_manufacturer) = 0;
  virtual ::ndk::ScopedAStatus setWpsModelName(const std::string& in_modelName) = 0;
  virtual ::ndk::ScopedAStatus setWpsModelNumber(const std::string& in_modelNumber) = 0;
  virtual ::ndk::ScopedAStatus setWpsSerialNumber(const std::string& in_serialNumber) = 0;
  virtual ::ndk::ScopedAStatus startWpsPbc(const std::string& in_groupIfName, const std::vector<uint8_t>& in_bssid) = 0;
  virtual ::ndk::ScopedAStatus startWpsPinDisplay(const std::string& in_groupIfName, const std::vector<uint8_t>& in_bssid, std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus startWpsPinKeypad(const std::string& in_groupIfName, const std::string& in_pin) = 0;
  virtual ::ndk::ScopedAStatus stopFind() = 0;
  virtual ::ndk::ScopedAStatus findOnSocialChannels(int32_t in_timeoutInSec) = 0;
  virtual ::ndk::ScopedAStatus findOnSpecificFrequency(int32_t in_freqInHz, int32_t in_timeoutInSec) = 0;
  virtual ::ndk::ScopedAStatus setVendorElements(::aidl::android::hardware::wifi::supplicant::P2pFrameTypeMask in_frameTypeMask, const std::vector<uint8_t>& in_vendorElemBytes) = 0;
  virtual ::ndk::ScopedAStatus configureEapolIpAddressAllocationParams(int32_t in_ipAddressGo, int32_t in_ipAddressMask, int32_t in_ipAddressStart, int32_t in_ipAddressEnd) = 0;
private:
  static std::shared_ptr<ISupplicantP2pIface> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
