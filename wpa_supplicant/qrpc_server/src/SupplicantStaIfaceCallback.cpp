/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant/supplicant_sta_iface_msg.h>

#include "SupplicantStaIfaceCallback.h"
#include "supplicant_event_callback.h"
#include <iostream>
#include <sstream>
#include <iomanip>

static inline void v2s(const vector<uint8_t>& v, string& s)
{
    std::stringstream buffer;
    buffer << " ";
    for (const auto& value : v) {
        buffer << std::setfill('0') << std::setw(2) << std::hex << value+0 << " ";
    }
    s = buffer.str();
}

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
int32_t SupplicantStaIfaceCallback::getIfaceInstanceId()
{
    return iface_instance_id_;
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAnqpQueryDone(const std::vector<uint8_t>& in_bssid, const ::aidl::android::hardware::wifi::supplicant::AnqpData& in_data, const ::aidl::android::hardware::wifi::supplicant::Hs20AnqpData& in_hs20Data)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAssociationRejected(const ::aidl::android::hardware::wifi::supplicant::AssociationRejectionData& in_assocRejectData)
{
    int32_t ifId = getIfaceInstanceId();
    ALOGI("Sending StaIface Event <onAssociationRejected> for ifaceId --> %d", ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAssociationRejectedInd(in_assocRejectData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAssociationRejected>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_ASSOCIATION_REJECTED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAuthenticationTimeout(const std::vector<uint8_t>& in_bssid)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;

    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onAuthenticationTimeout>: (%s) for ifaceId --> %d", temp.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAuthenticationTimeoutInd(in_bssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAuthenticationTimeout>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_AUTHENTICATION_TIMEOUT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAuxiliarySupplicantEvent(::aidl::android::hardware::wifi::supplicant::AuxiliarySupplicantEventCode in_eventCode, const std::vector<uint8_t>& in_bssid, const std::string& in_reasonString)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssTmHandlingDone(const ::aidl::android::hardware::wifi::supplicant::BssTmData& in_tmData)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssidChanged(::aidl::android::hardware::wifi::supplicant::BssidChangeReason in_reason, const std::vector<uint8_t>& in_bssid)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onBssidChanged>: (%s), (%s) for ifaceId --> %d", toString(in_reason).c_str(), temp.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnBssidChangedInd(in_reason, in_bssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onBssidChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_BSSID_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDisconnected(const std::vector<uint8_t>& in_bssid, bool in_locallyGenerated, ::aidl::android::hardware::wifi::supplicant::StaIfaceReasonCode in_reasonCode)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onDisconnected>: (%s), (%d) for ifaceId --> %d", temp.c_str(), in_locallyGenerated, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDisconnectedInd(in_bssid, in_locallyGenerated, in_reasonCode, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDisconnected>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DISCONNECTED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppFailure(::aidl::android::hardware::wifi::supplicant::DppFailureCode in_code, const std::string& in_ssid, const std::string& in_channelList, const std::vector<char16_t>& in_bandList)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppProgress(::aidl::android::hardware::wifi::supplicant::DppProgressCode in_code)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccess(::aidl::android::hardware::wifi::supplicant::DppEventType in_event)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccessConfigReceived(const std::vector<uint8_t>& in_ssid, const std::string& in_password, const std::vector<uint8_t>& in_psk, ::aidl::android::hardware::wifi::supplicant::DppAkm in_securityAkm, const ::aidl::android::hardware::wifi::supplicant::DppConnectionKeys& in_dppConnectionKeys)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccessConfigSent()
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onEapFailure(const std::vector<uint8_t>& in_bssid, int32_t in_errorCode)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onExtRadioWorkStart(int32_t in_id)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onExtRadioWorkTimeout(int32_t in_id)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20DeauthImminentNotice(const std::vector<uint8_t>& in_bssid, int32_t in_reasonCode, int32_t in_reAuthDelayInSec, const std::string& in_url)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20IconQueryDone(const std::vector<uint8_t>& in_bssid, const std::string& in_fileName, const std::vector<uint8_t>& in_data)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20SubscriptionRemediation(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::OsuMethod in_osuMethod, const std::string& in_url)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20TermsAndConditionsAcceptanceRequestedNotification(const std::vector<uint8_t>& in_bssid, const std::string& in_url)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onHs20TermsAndConditionsAcceptanceRequestedNotification>: (%s), (%s) for ifaceId --> %d", temp.c_str(), in_url.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnHs20TermsAndConditionsAcceptanceRequestedNotificationInd(in_bssid, in_url, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onHs20TermsAndConditionsAcceptanceRequestedNotification>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_HS20_TERMS_AND_CONDITIONS_ACCEPTANCE_REQUESTED_NOTIFICATION_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkAdded(int32_t in_id)
{
    ALOGI("Sending StaIface Event <onNetworkAdded>");
    /*This is not used. Result is already sent back via status in create req.*/
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkNotFound(const std::vector<uint8_t>& in_ssid)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;
    v2s(in_ssid, temp);
    ALOGI("Sending StaIface Event <onNetworkNotFound>: (%s) for ifaceId --> %d", temp.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnNetworkNotFoundInd(in_ssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onNetworkNotFound>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_NETWORK_NOT_FOUND_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkRemoved(int32_t in_id)
{
    int32_t ifId = getIfaceInstanceId();
    ALOGI("Sending StaIface Event <onNetworkRemoved>: (%d) for ifaceId --> %d", in_id, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnNetworkRemovedInd(in_id, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onNetworkRemoved>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_NETWORK_REMOVED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onPmkCacheAdded(int64_t in_expirationTimeInSec, const std::vector<uint8_t>& in_serializedEntry)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onStateChanged(::aidl::android::hardware::wifi::supplicant::StaIfaceCallbackState in_newState, const std::vector<uint8_t>& in_bssid, int32_t in_id, const std::vector<uint8_t>& in_ssid, bool in_filsHlpSent)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventFail(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::WpsConfigError in_configError, ::aidl::android::hardware::wifi::supplicant::WpsErrorIndication in_errorInd)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventPbcOverlap()
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventSuccess()
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyReset()
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyRequest(int32_t in_qosPolicyRequestId, const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyData>& in_qosPolicyData)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onMloLinksInfoChanged(::aidl::android::hardware::wifi::supplicant::ISupplicantStaIfaceCallback::MloLinkInfoChangeReason in_reason)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppConfigReceived(const ::aidl::android::hardware::wifi::supplicant::DppConfigurationData& in_configData)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppConnectionStatusResultSent(::aidl::android::hardware::wifi::supplicant::DppStatusErrorCode in_code)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssFrequencyChanged(int32_t in_frequencyMhz)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onSupplicantStateChanged(const ::aidl::android::hardware::wifi::supplicant::SupplicantStateChangeData& in_stateChangeData)
{
    int32_t ifId = getIfaceInstanceId();
    ALOGI("Sending StaIface Event <onSupplicantStateChanged>: (%s) for ifaceId --> %d", toString(in_stateChangeData.newState).c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnSupplicantStateChangedInd(in_stateChangeData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onSupplicantStateChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_SUPPLICANT_STATE_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyResponseForScs(const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsResponseStatus>& in_qosPolicyScsResponseStatus)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onPmkSaCacheAdded(const ::aidl::android::hardware::wifi::supplicant::PmkSaCacheData& in_pmkSaData)
{
    int32_t ifId = getIfaceInstanceId();
    std::string temp;
    v2s(in_pmkSaData.serializedEntry, temp);
    ALOGI("Sending StaIface Event <onPmkSaCacheAdded>: (%s) for ifaceId --> %d", temp.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnPmkSaCacheAddedInd(in_pmkSaData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onPmkSaCacheAdded>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_PMK_SA_CACHE_ADDED_IND, data);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl

