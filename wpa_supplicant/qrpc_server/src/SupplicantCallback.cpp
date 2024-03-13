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
    LOG(INFO) << "Sending Supplicant Event <onInterfaceCreated>";
    /*This is not used. Result is already sent back via status in create req.*/
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus SupplicantCallback::onInterfaceRemoved(const std::string& in_ifaceName)
{
    LOG(INFO) << "Sending Supplicant Event <onInterfaceRemoved>: (" << in_ifaceName <<")";
#if 0
    std::vector<uint8_t> data;
    if (!SupplicantSerializeOnInterfaceRemovedInd(in_ifaceName, data)) {
        LOG(ERROR) << "[Fail] Serializing Supplicant Event <onInterfaceRemoved>.";
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    return SupplicantSendEvent(SUPPLICANT_ON_INTERFACE_REMOVED_IND, data);
#else
    return ndk::ScopedAStatus::ok();
#endif
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
