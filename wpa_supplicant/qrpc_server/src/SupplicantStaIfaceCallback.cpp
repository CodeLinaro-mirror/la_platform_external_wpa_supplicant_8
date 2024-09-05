/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant/supplicant_sta_iface_msg.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "SupplicantStaIfaceCallback.h"
#include "supplicant_event_callback.h"

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
uint16_t SupplicantStaIfaceCallback::getIfaceInstanceId()
{
    return iface_instance_id_;
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAnqpQueryDone(const std::vector<uint8_t>& in_bssid, const ::aidl::android::hardware::wifi::supplicant::AnqpData& in_data, const ::aidl::android::hardware::wifi::supplicant::Hs20AnqpData& in_hs20Data)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onAnqpQueryDone> (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAnqpQueryDoneInd(in_bssid, in_data, in_hs20Data, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAnqpQueryDone>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_ANQP_QUERY_DONE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAssociationRejected(const ::aidl::android::hardware::wifi::supplicant::AssociationRejectionData& in_assocRejectData)
{
    uint16_t ifaceId = getIfaceInstanceId();
    ALOGI("Sending StaIface Event <onAssociationRejected> (%s) for ifaceId --> %d", toString(in_assocRejectData.statusCode).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAssociationRejectedInd(in_assocRejectData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAssociationRejected>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_ASSOCIATION_REJECTED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAuthenticationTimeout(const std::vector<uint8_t>& in_bssid)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onAuthenticationTimeout>: (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAuthenticationTimeoutInd(in_bssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAuthenticationTimeout>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_AUTHENTICATION_TIMEOUT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onAuxiliarySupplicantEvent(::aidl::android::hardware::wifi::supplicant::AuxiliarySupplicantEventCode in_eventCode, const std::vector<uint8_t>& in_bssid, const std::string& in_reasonString)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onAuxiliarySupplicantEvent>: (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnAuxiliarySupplicantEventInd(in_eventCode, in_bssid, in_reasonString, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onAuxiliarySupplicantEvent>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_AUXILIARY_SUPPLICANT_EVENT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssTmHandlingDone(const ::aidl::android::hardware::wifi::supplicant::BssTmData& in_tmData)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onBssTmHandlingDone>: (%s) for ifaceId --> %d", toString(in_tmData.status).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnBssTmHandlingDoneInd(in_tmData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onBssTmHandlingDone>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_BSS_TM_HANDLING_DONE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssidChanged(::aidl::android::hardware::wifi::supplicant::BssidChangeReason in_reason, const std::vector<uint8_t>& in_bssid)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onBssidChanged>: (%s), (%s) for ifaceId --> %d", toString(in_reason).c_str(), temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnBssidChangedInd(in_reason, in_bssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onBssidChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_BSSID_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDisconnected(const std::vector<uint8_t>& in_bssid, bool in_locallyGenerated, ::aidl::android::hardware::wifi::supplicant::StaIfaceReasonCode in_reasonCode)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onDisconnected>: (%s), (%d) for ifaceId --> %d", temp.c_str(), in_locallyGenerated, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDisconnectedInd(in_bssid, in_locallyGenerated, in_reasonCode, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDisconnected>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DISCONNECTED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppFailure(::aidl::android::hardware::wifi::supplicant::DppFailureCode in_code, const std::string& in_ssid, const std::string& in_channelList, const std::vector<char16_t>& in_bandList)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppFailure>: (%s), (%s) for ifaceId --> %d", in_ssid.c_str(), toString(in_code).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppFailureInd(in_code, in_ssid, in_channelList, in_bandList, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppFailure>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_FAILURE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppProgress(::aidl::android::hardware::wifi::supplicant::DppProgressCode in_code)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppProgress>: (%s) for ifaceId --> %d", toString(in_code).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppProgressInd(in_code, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppProgress>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_PROGRESS_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccess(::aidl::android::hardware::wifi::supplicant::DppEventType in_event)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppSuccess>: (%s) for ifaceId --> %d", toString(in_event).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppSuccessInd(in_event, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppSuccess>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_SUCCESS_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccessConfigReceived(const std::vector<uint8_t>& in_ssid, const std::string& in_password, const std::vector<uint8_t>& in_psk, ::aidl::android::hardware::wifi::supplicant::DppAkm in_securityAkm, const ::aidl::android::hardware::wifi::supplicant::DppConnectionKeys& in_dppConnectionKeys)
{
//    uint16_t ifaceId = getIfaceInstanceId();

//    std::string temp;
//    v2s(in_ssid, temp);
//    ALOGI("Sending StaIface Event <onDppSuccessConfigReceived>: (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

//    std::vector<uint8_t> data;
//    if (!SupplicantStaIfaceSerializeOnDppSuccessConfigReceivedInd(in_ssid, in_password, in_psk, in_securityAkm, in_dppConnectionKeys, data)) {
//        ALOGE("[Fail] Serializing StaIface Event <onDppSuccessConfigReceived>");
//        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
//    }

//    SupplicantAddInstanceId(data, ifaceId);

//    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_SUCCESS_CONFIG_RECEIVED_IND, data);
    ALOGI("Deprecated Event <onPmkCacheAdded> not supported");

    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppSuccessConfigSent()
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppSuccessConfigSent> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppSuccessConfigSentInd(data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppSuccessConfigSent>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_SUCCESS_CONFIG_SENT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onEapFailure(const std::vector<uint8_t>& in_bssid, int32_t in_errorCode)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onEapFailure>: (%s), (%d) for ifaceId --> %d", temp.c_str(), in_errorCode, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnEapFailureInd(in_bssid, in_errorCode, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onEapFailure>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_EAP_FAILURE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onExtRadioWorkStart(int32_t in_id)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onExtRadioWorkStart>: (%d) for ifaceId --> %d", in_id, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnExtRadioWorkStartInd(in_id, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onExtRadioWorkStart>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_EXT_RADIO_WORK_START_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onExtRadioWorkTimeout(int32_t in_id)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onExtRadioWorkTimeout>: (%d) for ifaceId --> %d", in_id, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnExtRadioWorkTimeoutInd(in_id, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onExtRadioWorkTimeout>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_EXT_RADIO_WORK_TIMEOUT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20DeauthImminentNotice(const std::vector<uint8_t>& in_bssid, int32_t in_reasonCode, int32_t in_reAuthDelayInSec, const std::string& in_url)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onHs20DeauthImminentNotice>: (%s), (%d), (%d), (%s) for ifaceId --> %d", temp.c_str(), in_reasonCode, in_reAuthDelayInSec, in_url.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnHs20DeauthImminentNoticeInd(in_bssid, in_reasonCode, in_reAuthDelayInSec, in_url, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onHs20DeauthImminentNotice>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_HS20_DEAUTH_IMMINENT_NOTICE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20IconQueryDone(const std::vector<uint8_t>& in_bssid, const std::string& in_fileName, const std::vector<uint8_t>& in_data)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onHs20IconQueryDone>: (%s), (%s) for ifaceId --> %d", temp.c_str(), in_fileName.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnHs20IconQueryDoneInd(in_bssid, in_fileName, in_data, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onHs20IconQueryDone>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_HS20_ICON_QUERY_DONE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20SubscriptionRemediation(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::OsuMethod in_osuMethod, const std::string& in_url)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onHs20SubscriptionRemediation>: (%s), (%s), (%s) for ifaceId --> %d", temp.c_str(), toString(in_osuMethod).c_str(), in_url.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnHs20SubscriptionRemediationInd(in_bssid, in_osuMethod, in_url, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onHs20SubscriptionRemediation>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_HS20_SUBSCRIPTION_REMEDIATION_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onHs20TermsAndConditionsAcceptanceRequestedNotification(const std::vector<uint8_t>& in_bssid, const std::string& in_url)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onHs20TermsAndConditionsAcceptanceRequestedNotification>: (%s), (%s) for ifaceId --> %d", temp.c_str(), in_url.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnHs20TermsAndConditionsAcceptanceRequestedNotificationInd(in_bssid, in_url, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onHs20TermsAndConditionsAcceptanceRequestedNotification>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_HS20_TERMS_AND_CONDITIONS_ACCEPTANCE_REQUESTED_NOTIFICATION_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkAdded(int32_t in_id)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onNetworkAdded>: (%d) for ifaceId --> %d", in_id, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnNetworkAddedInd(in_id, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onNetworkAdded>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_NETWORK_ADDED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkNotFound(const std::vector<uint8_t>& in_ssid)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_ssid, temp);
    ALOGI("Sending StaIface Event <onNetworkNotFound>: (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnNetworkNotFoundInd(in_ssid, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onNetworkNotFound>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_NETWORK_NOT_FOUND_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onNetworkRemoved(int32_t in_id)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onNetworkRemoved>: (%d) for ifaceId --> %d", in_id, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnNetworkRemovedInd(in_id, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onNetworkRemoved>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_NETWORK_REMOVED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onPmkCacheAdded(int64_t in_expirationTimeInSec, const std::vector<uint8_t>& in_serializedEntry)
{
//    uint16_t ifaceId = getIfaceInstanceId();

//    std::string temp;
//    v2s(in_serializedEntry, temp);
//    ALOGI("Sending StaIface Event <onPmkCacheAdded>: (%d), (%s) for ifaceId --> %d", in_expirationTimeInSec, temp.c_str(), ifaceId);

//    std::vector<uint8_t> data;
//    if (!SupplicantStaIfaceSerializeOnPmkCacheAddedInd(in_expirationTimeInSec, in_serializedEntry, data)) {
//        ALOGE("[Fail] Serializing StaIface Event <onPmkCacheAdded>");
//        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
//    }

//    SupplicantAddInstanceId(data, ifaceId);

//    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_PMK_CACHE_ADDED_IND, data);
    ALOGI("Deprecated Event <onPmkCacheAdded> not supported");

    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onStateChanged(::aidl::android::hardware::wifi::supplicant::StaIfaceCallbackState in_newState, const std::vector<uint8_t>& in_bssid, int32_t in_id, const std::vector<uint8_t>& in_ssid, bool in_filsHlpSent)
{
    ALOGI("Deprecated Event <onStateChanged> not supported");

    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventFail(const std::vector<uint8_t>& in_bssid, ::aidl::android::hardware::wifi::supplicant::WpsConfigError in_configError, ::aidl::android::hardware::wifi::supplicant::WpsErrorIndication in_errorInd)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_bssid, temp);
    ALOGI("Sending StaIface Event <onWpsEventFail>: (%s), (%s), (%s) for ifaceId --> %d", temp.c_str(), toString(in_configError).c_str(), toString(in_errorInd).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnWpsEventFailInd(in_bssid, in_configError, in_errorInd, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onWpsEventFail>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_WPS_EVENT_FAIL_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventPbcOverlap()
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onWpsEventPbcOverlap> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnWpsEventPbcOverlapInd(data)) {
        ALOGE("[Fail] Serializing StaIface Event <onWpsEventPbcOverlap>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_WPS_EVENT_PBC_OVERLAP_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onWpsEventSuccess()
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onWpsEventSuccess> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnWpsEventSuccessInd(data)) {
        ALOGE("[Fail] Serializing StaIface Event <onWpsEventSuccess>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_WPS_EVENT_SUCCESS_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyReset()
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onQosPolicyReset> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnQosPolicyResetInd(data)) {
        ALOGE("[Fail] Serializing StaIface Event <onQosPolicyReset>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_QOS_POLICY_RESET_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyRequest(int32_t in_qosPolicyRequestId, const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyData>& in_qosPolicyData)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onQosPolicyRequest>: (%d) for ifaceId --> %d", in_qosPolicyRequestId, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnQosPolicyRequestInd(in_qosPolicyRequestId, in_qosPolicyData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onQosPolicyRequest>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_QOS_POLICY_REQUEST_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onMloLinksInfoChanged(::aidl::android::hardware::wifi::supplicant::ISupplicantStaIfaceCallback::MloLinkInfoChangeReason in_reason)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onMloLinksInfoChanged>: (%s) for ifaceId --> %d", toString(in_reason).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnMloLinksInfoChangedInd(in_reason, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onMloLinksInfoChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_MLO_LINKS_INFO_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppConfigReceived(const ::aidl::android::hardware::wifi::supplicant::DppConfigurationData& in_configData)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppConfigReceived> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppConfigReceivedInd(in_configData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppConfigReceived>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_CONFIG_RECEIVED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onDppConnectionStatusResultSent(::aidl::android::hardware::wifi::supplicant::DppStatusErrorCode in_code)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onDppConnectionStatusResultSent> (%s) for ifaceId --> %d", toString(in_code).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnDppConnectionStatusResultSentInd(in_code, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onDppConnectionStatusResultSent>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_DPP_CONNECTION_STATUS_RESULT_SENT_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onBssFrequencyChanged(int32_t in_frequencyMhz)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onBssFrequencyChanged> (%d) for ifaceId --> %d", in_frequencyMhz, ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnBssFrequencyChangedInd(in_frequencyMhz, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onBssFrequencyChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_BSS_FREQUENCY_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onSupplicantStateChanged(const ::aidl::android::hardware::wifi::supplicant::SupplicantStateChangeData& in_stateChangeData)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onSupplicantStateChanged>: (%s) for ifaceId --> %d", toString(in_stateChangeData.newState).c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnSupplicantStateChangedInd(in_stateChangeData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onSupplicantStateChanged>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_SUPPLICANT_STATE_CHANGED_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onQosPolicyResponseForScs(const std::vector<::aidl::android::hardware::wifi::supplicant::QosPolicyScsResponseStatus>& in_qosPolicyScsResponseStatus)
{
    uint16_t ifaceId = getIfaceInstanceId();

    ALOGI("Sending StaIface Event <onQosPolicyResponseForScs> for ifaceId --> %d", ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnQosPolicyResponseForScsInd(in_qosPolicyScsResponseStatus, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onQosPolicyResponseForScs>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_QOS_POLICY_RESPONSE_FOR_SCS_IND, data);
}

::ndk::ScopedAStatus SupplicantStaIfaceCallback::onPmkSaCacheAdded(const ::aidl::android::hardware::wifi::supplicant::PmkSaCacheData& in_pmkSaData)
{
    uint16_t ifaceId = getIfaceInstanceId();

    std::string temp;
    v2s(in_pmkSaData.serializedEntry, temp);
    ALOGI("Sending StaIface Event <onPmkSaCacheAdded>: (%s) for ifaceId --> %d", temp.c_str(), ifaceId);

    std::vector<uint8_t> data;
    if (!SupplicantStaIfaceSerializeOnPmkSaCacheAddedInd(in_pmkSaData, data)) {
        ALOGE("[Fail] Serializing StaIface Event <onPmkSaCacheAdded>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data, ifaceId);

    return SupplicantSendEvent(SUPPLICANT_STA_IFACE_ON_PMK_SA_CACHE_ADDED_IND, data);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl

