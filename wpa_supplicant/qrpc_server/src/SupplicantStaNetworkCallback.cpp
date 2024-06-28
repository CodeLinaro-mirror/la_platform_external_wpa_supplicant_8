/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant/supplicant_sta_network_msg.h>

#include "SupplicantStaNetworkCallback.h"
#include "supplicant_event_callback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
int32_t SupplicantStaNetworkCallback::getIfaceInstanceId()
{
    return iface_instance_id_;
}

int32_t SupplicantStaNetworkCallback::getNetworkInstanceId()
{
    return network_instance_id_;
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onNetworkEapIdentityRequest()
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onNetworkEapIdentityRequest> for network -> %d of iface --> %d", nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnNetworkEapIdentityRequestInd(data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onNetworkEapIdentityRequest>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_NETWORK_EAP_IDENTITY_REQUEST_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onNetworkEapSimGsmAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimGsmAuthParams& in_params)
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onNetworkEapSimGsmAuthRequest> for network -> %d of iface --> %d", nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnNetworkEapSimGsmAuthRequestInd(in_params, data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onNetworkEapSimGsmAuthRequest>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_NETWORK_EAP_SIM_GSM_AUTH_REQUEST_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onNetworkEapSimUmtsAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimUmtsAuthParams& in_params)
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onNetworkEapSimUmtsAuthRequest> for network -> %d of iface --> %d", nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnNetworkEapSimUmtsAuthRequestInd(in_params, data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onNetworkEapSimUmtsAuthRequest>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_NETWORK_EAP_SIM_UMTS_AUTH_REQUEST_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onTransitionDisable(::aidl::android::hardware::wifi::supplicant::TransitionDisableIndication in_ind)
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onTransitionDisable> (%d) for network -> %d of iface --> %d", in_ind, nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnTransitionDisableInd(in_ind, data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onTransitionDisable>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_TRANSITION_DISABLE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onServerCertificateAvailable(int32_t in_depth, const std::vector<uint8_t>& in_subject, const std::vector<uint8_t>& in_certHash, const std::vector<uint8_t>& in_certBlob)
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onServerCertificateAvailable> (%d) for network -> %d of iface --> %d", in_depth, nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnServerCertificateAvailableInd(in_depth, in_subject, in_certHash, in_certBlob, data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onServerCertificateAvailable>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_SERVER_CERTIFICATE_AVAILABLE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onPermanentIdReqDenied()
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();

    ALOGI("Sending StaNetwork Event <onPermanentIdReqDenied> for network -> %d of iface --> %d", nwId, ifId);

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnPermanentIdReqDeniedInd(data)) {
        ALOGE("[Fail] Serializing StaNetwork Event <onPermanentIdReqDenied>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_PERMANENT_ID_REQ_DENIED_IND, data);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
