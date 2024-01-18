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
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onNetworkEapSimGsmAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimGsmAuthParams& in_params)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onNetworkEapSimUmtsAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimUmtsAuthParams& in_params)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onTransitionDisable(::aidl::android::hardware::wifi::supplicant::TransitionDisableIndication in_ind)
{
    int32_t ifId = getIfaceInstanceId();
    int32_t nwId = getNetworkInstanceId();
    LOG(INFO) << "Sending StaNetwork Event <onTransitionDisable> for network -> " << nwId <<" of iface --> "<< ifId;

    std::vector<uint8_t> data;
    if (!SupplicantStaNetworkSerializeOnTransitionDisableInd(in_ind, data)) {
        LOG(ERROR) << "[Fail] Serializing StaNetwork Event <onTransitionDisable>.";
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);
    ADD_INT32_TO_VECTOR(nwId, data);

    return SupplicantSendEvent(SUPPLICANT_STA_NETWORK_ON_TRANSITION_DISABLE_IND, data);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onServerCertificateAvailable(int32_t in_depth, const std::vector<uint8_t>& in_subject, const std::vector<uint8_t>& in_certHash, const std::vector<uint8_t>& in_certBlob)
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus SupplicantStaNetworkCallback::onPermanentIdReqDenied()
{
    return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
