/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant/supplicant_msg.h>

#include "SupplicantCallback.h"
#include "supplicant_event_callback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
::ndk::ScopedAStatus SupplicantCallback::onInterfaceCreated(const std::string& in_ifaceName)
{
    ALOGI("Sending Supplicant Event <onInterfaceCreated>: %s", in_ifaceName.c_str());

    std::vector<uint8_t> data;
    if (!SupplicantSerializeOnInterfaceCreatedInd(in_ifaceName, data)) {
        ALOGE("[Fail] Serializing Supplicant Event <onInterfaceRemoved>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data);

    return SupplicantSendEvent(SUPPLICANT_ON_INTERFACE_CREATED_IND, data);
}

::ndk::ScopedAStatus SupplicantCallback::onInterfaceRemoved(const std::string& in_ifaceName)
{
    ALOGI("Sending Supplicant Event <onInterfaceRemoved>: %s", in_ifaceName.c_str());

    std::vector<uint8_t> data;
    if (!SupplicantSerializeOnInterfaceRemovedInd(in_ifaceName, data)) {
        ALOGE("[Fail] Serializing Supplicant Event <onInterfaceRemoved>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    SupplicantAddInstanceId(data);

    return SupplicantSendEvent(SUPPLICANT_ON_INTERFACE_REMOVED_IND, data);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
