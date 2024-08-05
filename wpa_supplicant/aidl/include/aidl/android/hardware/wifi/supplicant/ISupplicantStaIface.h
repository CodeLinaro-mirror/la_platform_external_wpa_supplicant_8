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
#include <aidl/android/hardware/wifi/supplicant/AnqpInfoId.h>
#include <aidl/android/hardware/wifi/supplicant/BtCoexistenceMode.h>
#include <aidl/android/hardware/wifi/supplicant/ConnectionCapabilities.h>
#include <aidl/android/hardware/wifi/supplicant/DppAkm.h>
#include <aidl/android/hardware/wifi/supplicant/DppCurve.h>
#include <aidl/android/hardware/wifi/supplicant/DppNetRole.h>
#include <aidl/android/hardware/wifi/supplicant/DppResponderBootstrapInfo.h>
#include <aidl/android/hardware/wifi/supplicant/Hs20AnqpSubtypes.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantStaIfaceCallback.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantStaNetwork.h>
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>
#include <aidl/android/hardware/wifi/supplicant/KeyMgmtMask.h>
#include <aidl/android/hardware/wifi/supplicant/MloLinksInfo.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyScsData.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyScsRequestStatus.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyStatus.h>
#include <aidl/android/hardware/wifi/supplicant/RxFilterType.h>
#include <aidl/android/hardware/wifi/supplicant/SignalPollResult.h>
#include <aidl/android/hardware/wifi/supplicant/WpaDriverCapabilitiesMask.h>
#include <aidl/android/hardware/wifi/supplicant/WpsConfigMethods.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class ConnectionCapabilities;
class DppResponderBootstrapInfo;
class ISupplicantStaIfaceCallback;
class ISupplicantStaNetwork;
class MloLinksInfo;
class QosPolicyScsData;
class QosPolicyScsRequestStatus;
class QosPolicyStatus;
class SignalPollResult;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantStaIface : public ::ndk::ICInterface {
public:
//  ISupplicantStaIface();
//  virtual ~ISupplicantStaIface();

  enum : int32_t { MAX_POLICIES_PER_QOS_SCS_REQUEST = 16 };

//  static bool setDefaultImpl(const std::shared_ptr<ISupplicantStaIface>& impl);
//  static const std::shared_ptr<ISupplicantStaIface>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus addDppPeerUri(const std::string& in_uri, int32_t* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addExtRadioWork(const std::string& in_name, int32_t in_freqInMhz, int32_t in_timeoutInSec, int32_t* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addNetwork(std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaNetwork>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addRxFilter(::aidl::android::hardware::wifi::supplicant::RxFilterType in_type) = 0;
  virtual ::ndk::ScopedAStatus cancelWps() = 0;
  virtual ::ndk::ScopedAStatus disconnect() = 0;
  virtual ::ndk::ScopedAStatus enableAutoReconnect(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus filsHlpAddRequest(const std::vector<uint8_t>& in_dst_mac, const std::vector<uint8_t>& in_pkt) = 0;
  virtual ::ndk::ScopedAStatus filsHlpFlushRequest() = 0;
  virtual ::ndk::ScopedAStatus generateDppBootstrapInfoForResponder(const std::vector<uint8_t>& in_macAddress, const std::string& in_deviceInfo, ::aidl::android::hardware::wifi::supplicant::DppCurve in_curve, ::aidl::android::hardware::wifi::supplicant::DppResponderBootstrapInfo* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus generateSelfDppConfiguration(const std::string& in_ssid, const std::vector<uint8_t>& in_privEcKey) = 0;
  virtual ::ndk::ScopedAStatus getConnectionCapabilities(::aidl::android::hardware::wifi::supplicant::ConnectionCapabilities* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getConnectionMloLinksInfo(::aidl::android::hardware::wifi::supplicant::MloLinksInfo* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getKeyMgmtCapabilities(::aidl::android::hardware::wifi::supplicant::KeyMgmtMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getMacAddress(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getName(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getNetwork(int32_t in_id, std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaNetwork>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getType(::aidl::android::hardware::wifi::supplicant::IfaceType* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getWpaDriverCapabilities(::aidl::android::hardware::wifi::supplicant::WpaDriverCapabilitiesMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus initiateAnqpQuery(const std::vector<uint8_t>& in_macAddress, const std::vector<::aidl::android::hardware::wifi::supplicant::AnqpInfoId>& in_infoElements, const std::vector<::aidl::android::hardware::wifi::supplicant::Hs20AnqpSubtypes>& in_subTypes) = 0;
  virtual ::ndk::ScopedAStatus initiateHs20IconQuery(const std::vector<uint8_t>& in_macAddress, const std::string& in_fileName) = 0;
  virtual ::ndk::ScopedAStatus initiateTdlsDiscover(const std::vector<uint8_t>& in_macAddress) = 0;
  virtual ::ndk::ScopedAStatus initiateTdlsSetup(const std::vector<uint8_t>& in_macAddress) = 0;
  virtual ::ndk::ScopedAStatus initiateTdlsTeardown(const std::vector<uint8_t>& in_macAddress) = 0;
  virtual ::ndk::ScopedAStatus initiateVenueUrlAnqpQuery(const std::vector<uint8_t>& in_macAddress) = 0;
  virtual ::ndk::ScopedAStatus listNetworks(std::vector<int32_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus reassociate() = 0;
  virtual ::ndk::ScopedAStatus reconnect() = 0;
//  virtual ::ndk::ScopedAStatus registerCallback(const std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaIfaceCallback>& in_callback) = 0;
  virtual ::ndk::ScopedAStatus setQosPolicyFeatureEnabled(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus sendQosPolicyResponse(int32_t in_qosPolicyRequestId, bool in_morePolicies, const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyStatus>& in_qosPolicyStatusList) = 0;
  virtual ::ndk::ScopedAStatus removeAllQosPolicies() = 0;
  virtual ::ndk::ScopedAStatus removeDppUri(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus removeExtRadioWork(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus removeNetwork(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus removeRxFilter(::aidl::android::hardware::wifi::supplicant::RxFilterType in_type) = 0;
  virtual ::ndk::ScopedAStatus setBtCoexistenceMode(::aidl::android::hardware::wifi::supplicant::BtCoexistenceMode in_mode) = 0;
  virtual ::ndk::ScopedAStatus setBtCoexistenceScanModeEnabled(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setCountryCode(const std::vector<uint8_t>& in_code) = 0;
  virtual ::ndk::ScopedAStatus setExternalSim(bool in_useExternalSim) = 0;
  virtual ::ndk::ScopedAStatus setMboCellularDataStatus(bool in_available) = 0;
  virtual ::ndk::ScopedAStatus setPowerSave(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setSuspendModeEnabled(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setWpsConfigMethods(::aidl::android::hardware::wifi::supplicant::WpsConfigMethods in_configMethods) = 0;
  virtual ::ndk::ScopedAStatus setWpsDeviceName(const std::string& in_name) = 0;
  virtual ::ndk::ScopedAStatus setWpsDeviceType(const std::vector<uint8_t>& in_type) = 0;
  virtual ::ndk::ScopedAStatus setWpsManufacturer(const std::string& in_manufacturer) = 0;
  virtual ::ndk::ScopedAStatus setWpsModelName(const std::string& in_modelName) = 0;
  virtual ::ndk::ScopedAStatus setWpsModelNumber(const std::string& in_modelNumber) = 0;
  virtual ::ndk::ScopedAStatus setWpsSerialNumber(const std::string& in_serialNumber) = 0;
  virtual ::ndk::ScopedAStatus startDppConfiguratorInitiator(int32_t in_peerBootstrapId, int32_t in_ownBootstrapId, const std::string& in_ssid, const std::string& in_password, const std::string& in_psk, ::aidl::android::hardware::wifi::supplicant::DppNetRole in_netRole, ::aidl::android::hardware::wifi::supplicant::DppAkm in_securityAkm, const std::vector<uint8_t>& in_privEcKey, std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus startDppEnrolleeInitiator(int32_t in_peerBootstrapId, int32_t in_ownBootstrapId) = 0;
  virtual ::ndk::ScopedAStatus startDppEnrolleeResponder(int32_t in_listenChannel) = 0;
  virtual ::ndk::ScopedAStatus startRxFilter() = 0;
  virtual ::ndk::ScopedAStatus startWpsPbc(const std::vector<uint8_t>& in_bssid) = 0;
  virtual ::ndk::ScopedAStatus startWpsPinDisplay(const std::vector<uint8_t>& in_bssid, std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus startWpsPinKeypad(const std::string& in_pin) = 0;
  virtual ::ndk::ScopedAStatus startWpsRegistrar(const std::vector<uint8_t>& in_bssid, const std::string& in_pin) = 0;
  virtual ::ndk::ScopedAStatus stopDppInitiator() = 0;
  virtual ::ndk::ScopedAStatus stopDppResponder(int32_t in_ownBootstrapId) = 0;
  virtual ::ndk::ScopedAStatus stopRxFilter() = 0;
  virtual ::ndk::ScopedAStatus getSignalPollResults(std::vector<::aidl::android::hardware::wifi::supplicant::SignalPollResult>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus addQosPolicyRequestForScs(const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsData>& in_qosPolicyData, std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsRequestStatus>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus removeQosPolicyForScs(const std::vector<uint8_t>& in_scsPolicyIds, std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsRequestStatus>* _aidl_return) = 0;
//private:
//  static std::shared_ptr<ISupplicantStaIface> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
