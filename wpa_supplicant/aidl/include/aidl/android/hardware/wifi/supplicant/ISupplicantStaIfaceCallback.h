/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <aidl/android/hardware/wifi/supplicant/AnqpData.h>
#include <aidl/android/hardware/wifi/supplicant/AssociationRejectionData.h>
#include <aidl/android/hardware/wifi/supplicant/AuxiliarySupplicantEventCode.h>
#include <aidl/android/hardware/wifi/supplicant/BssTmData.h>
#include <aidl/android/hardware/wifi/supplicant/BssidChangeReason.h>
#include <aidl/android/hardware/wifi/supplicant/DppAkm.h>
#include <aidl/android/hardware/wifi/supplicant/DppConfigurationData.h>
#include <aidl/android/hardware/wifi/supplicant/DppConnectionKeys.h>
#include <aidl/android/hardware/wifi/supplicant/DppEventType.h>
#include <aidl/android/hardware/wifi/supplicant/DppFailureCode.h>
#include <aidl/android/hardware/wifi/supplicant/DppProgressCode.h>
#include <aidl/android/hardware/wifi/supplicant/DppStatusErrorCode.h>
#include <aidl/android/hardware/wifi/supplicant/Hs20AnqpData.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantStaIfaceCallback.h>
#include <aidl/android/hardware/wifi/supplicant/OsuMethod.h>
#include <aidl/android/hardware/wifi/supplicant/PmkSaCacheData.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyData.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyScsResponseStatus.h>
#include <aidl/android/hardware/wifi/supplicant/StaIfaceCallbackState.h>
#include <aidl/android/hardware/wifi/supplicant/StaIfaceReasonCode.h>
#include <aidl/android/hardware/wifi/supplicant/SupplicantStateChangeData.h>
#include <aidl/android/hardware/wifi/supplicant/WpsConfigError.h>
#include <aidl/android/hardware/wifi/supplicant/WpsErrorIndication.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class AnqpData;
class AssociationRejectionData;
class BssTmData;
class DppConfigurationData;
class DppConnectionKeys;
class Hs20AnqpData;
class PmkSaCacheData;
class QosPolicyData;
class QosPolicyScsResponseStatus;
class SupplicantStateChangeData;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantStaIfaceCallback : public ::ndk::ICInterface {
public:
  enum class MloLinkInfoChangeReason : int32_t {
    TID_TO_LINK_MAP = 0,
    MULTI_LINK_RECONFIG_AP_REMOVAL = 1,
  };

//  static bool setDefaultImpl(const std::shared_ptr<ISupplicantStaIfaceCallback>& impl);
//  static const std::shared_ptr<ISupplicantStaIfaceCallback>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus onAnqpQueryDone(const std::vector<uint8_t>& in_bssid, const ::aidl::android::hardware::wifi::supplicant::AnqpData& in_data, const ::aidl::android::hardware::wifi::supplicant::Hs20AnqpData& in_hs20Data) = 0;
  virtual ::ndk::ScopedAStatus onAssociationRejected(const ::aidl::android::hardware::wifi::supplicant::AssociationRejectionData& in_assocRejectData) = 0;
  virtual ::ndk::ScopedAStatus onAuthenticationTimeout(const std::vector<uint8_t>& in_bssid) = 0;
  virtual ::ndk::ScopedAStatus onAuxiliarySupplicantEvent(::aidl::android::hardware::wifi::supplicant::AuxiliarySupplicantEventCode in_eventCode, const std::vector<uint8_t>& in_bssid, const std::string& in_reasonString) = 0;
  virtual ::ndk::ScopedAStatus onBssTmHandlingDone(const ::aidl::android::hardware::wifi::supplicant::BssTmData& in_tmData) = 0;
  virtual ::ndk::ScopedAStatus onBssidChanged(::aidl::android::hardware::wifi::supplicant::BssidChangeReason in_reason, const std::vector<uint8_t>& in_bssid) = 0;
  virtual ::ndk::ScopedAStatus onDisconnected(const std::vector<uint8_t>& in_bssid, bool in_locallyGenerated, ::aidl::android::hardware::wifi::supplicant::StaIfaceReasonCode in_reasonCode) = 0;
  virtual ::ndk::ScopedAStatus onDppFailure(::aidl::android::hardware::wifi::supplicant::DppFailureCode in_code, const std::string& in_ssid, const std::string& in_channelList, const std::vector<char16_t>& in_bandList) = 0;
  virtual ::ndk::ScopedAStatus onDppProgress(::aidl::android::hardware::wifi::supplicant::DppProgressCode in_code) = 0;
  virtual ::ndk::ScopedAStatus onDppSuccess(::aidl::android::hardware::wifi::supplicant::DppEventType in_event) = 0;
  virtual ::ndk::ScopedAStatus onDppSuccessConfigReceived(const std::vector<uint8_t>& in_ssid, const std::string& in_password, const std::vector<uint8_t>& in_psk, ::aidl::android::hardware::wifi::supplicant::DppAkm in_securityAkm, const ::aidl::android::hardware::wifi::supplicant::DppConnectionKeys& in_dppConnectionKeys) __attribute__((deprecated("This callback is deprecated from AIDL v2, newer HAL should call onDppConfigReceived."))) = 0;
  virtual ::ndk::ScopedAStatus onDppSuccessConfigSent() = 0;
  virtual ::ndk::ScopedAStatus onEapFailure(const std::vector<uint8_t>& in_bssid, int32_t in_errorCode) = 0;
  virtual ::ndk::ScopedAStatus onExtRadioWorkStart(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus onExtRadioWorkTimeout(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus onHs20DeauthImminentNotice(const std::vector<uint8_t>& in_bssid, int32_t in_reasonCode, int32_t in_reAuthDelayInSec, const std::string& in_url) = 0;
  virtual ::ndk::ScopedAStatus onHs20IconQueryDone(const std::vector<uint8_t>& in_bssid, const std::string& in_fileName, const std::vector<uint8_t>& in_data) = 0;
  virtual ::ndk::ScopedAStatus onHs20SubscriptionRemediation(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::OsuMethod in_osuMethod, const std::string& in_url) = 0;
  virtual ::ndk::ScopedAStatus onHs20TermsAndConditionsAcceptanceRequestedNotification(const std::vector<uint8_t>& in_bssid, const std::string& in_url) = 0;
  virtual ::ndk::ScopedAStatus onNetworkAdded(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus onNetworkNotFound(const std::vector<uint8_t>& in_ssid) = 0;
  virtual ::ndk::ScopedAStatus onNetworkRemoved(int32_t in_id) = 0;
  virtual ::ndk::ScopedAStatus onPmkCacheAdded(int64_t in_expirationTimeInSec, const std::vector<uint8_t>& in_serializedEntry) __attribute__((deprecated("use onPmkSaCacheAdded() instead."))) = 0;
  virtual ::ndk::ScopedAStatus onStateChanged(::aidl::android::hardware::wifi::supplicant::StaIfaceCallbackState in_newState, const std::vector<uint8_t>& in_bssid, int32_t in_id, const std::vector<uint8_t>& in_ssid, bool in_filsHlpSent) __attribute__((deprecated("This callback is deprecated from AIDL v2, newer HAL should call onSupplicantStateChanged()"))) = 0;
  virtual ::ndk::ScopedAStatus onWpsEventFail(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::WpsConfigError in_configError, ::aidl::android::hardware::wifi::supplicant::WpsErrorIndication in_errorInd) = 0;
  virtual ::ndk::ScopedAStatus onWpsEventPbcOverlap() = 0;
  virtual ::ndk::ScopedAStatus onWpsEventSuccess() = 0;
  virtual ::ndk::ScopedAStatus onQosPolicyReset() = 0;
  virtual ::ndk::ScopedAStatus onQosPolicyRequest(int32_t in_qosPolicyRequestId, const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyData>& in_qosPolicyData) = 0;
  virtual ::ndk::ScopedAStatus onMloLinksInfoChanged(::aidl::android::hardware::wifi::supplicant::ISupplicantStaIfaceCallback::MloLinkInfoChangeReason in_reason) = 0;
  virtual ::ndk::ScopedAStatus onDppConfigReceived(const ::aidl::android::hardware::wifi::supplicant::DppConfigurationData& in_configData) = 0;
  virtual ::ndk::ScopedAStatus onDppConnectionStatusResultSent(::aidl::android::hardware::wifi::supplicant::DppStatusErrorCode in_code) = 0;
  virtual ::ndk::ScopedAStatus onBssFrequencyChanged(int32_t in_frequencyMhz) = 0;
  virtual ::ndk::ScopedAStatus onSupplicantStateChanged(const ::aidl::android::hardware::wifi::supplicant::SupplicantStateChangeData& in_stateChangeData) = 0;
  virtual ::ndk::ScopedAStatus onQosPolicyResponseForScs(const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsResponseStatus>& in_qosPolicyScsResponseStatus) = 0;
  virtual ::ndk::ScopedAStatus onPmkSaCacheAdded(const ::aidl::android::hardware::wifi::supplicant::PmkSaCacheData& in_pmkSaData) = 0;
//private:
//  static std::shared_ptr<ISupplicantStaIfaceCallback> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
[[nodiscard]] static inline std::string toString(ISupplicantStaIfaceCallback::MloLinkInfoChangeReason val) {
  switch(val) {
  case ISupplicantStaIfaceCallback::MloLinkInfoChangeReason::TID_TO_LINK_MAP:
    return "TID_TO_LINK_MAP";
  case ISupplicantStaIfaceCallback::MloLinkInfoChangeReason::MULTI_LINK_RECONFIG_AP_REMOVAL:
    return "MULTI_LINK_RECONFIG_AP_REMOVAL";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
