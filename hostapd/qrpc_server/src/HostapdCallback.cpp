/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/hostapd/hostapd_msg.h>
#include "HostapdCallback.h"
#include "hostapd_event_callback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {

::ndk::ScopedAStatus HostapdCallback::onApInstanceInfoChanged(const ::aidl::android::hardware::wifi::hostapd::ApInfo& in_apInfo)
{
    std::vector<uint8_t> data;
    if (!HostapdSerializeOnApInstanceInfoChangedInd(in_apInfo, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onApInstanceInfoChanged>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_ON_AP_INSTANCE_INFO_CHANGED_IND, data);
}

::ndk::ScopedAStatus HostapdCallback::onConnectedClientsChanged(const ::aidl::android::hardware::wifi::hostapd::ClientInfo& in_clientInfo)
{
    std::vector<uint8_t> data;
    if (!HostapdSerializeOnConnectedClientsChangedInd(in_clientInfo, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onConnectedClientsChanged>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_ON_CONNECTED_CLIENTS_CHANGED_IND, data);
}

::ndk::ScopedAStatus HostapdCallback::onFailure(const std::string& in_ifaceName, const std::string& in_instanceName)
{
    std::vector<uint8_t> data;
    if (!HostapdSerializeOnFailureInd(in_ifaceName,in_instanceName, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onFailure>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_ON_FAILURE_IND, data);
}

}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
