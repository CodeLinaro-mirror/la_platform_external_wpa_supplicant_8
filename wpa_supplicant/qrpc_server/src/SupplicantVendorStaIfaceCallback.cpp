/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant_vendor/supplicant_vendor_sta_iface_msg.h>

#include "SupplicantVendorStaIfaceCallback.h"
#include "supplicant_event_callback.h"

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {

using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;

int32_t SupplicantVendorStaIfaceCallback::getIfaceInstanceId()
{
    return iface_instance_id_;
}

::ndk::ScopedAStatus SupplicantVendorStaIfaceCallback::onCtrlEvent(const std::string& in_ifaceName, const std::string& in_eventStr)
{
    int32_t ifId = getIfaceInstanceId();
    ALOGI("Sending Vendor StaIface Event <onCtrlEvent>: (%s) of (%s) for ifaceId --> %d", in_eventStr.c_str(), in_ifaceName.c_str(), ifId);

    std::vector<uint8_t> data;
    if (!SupplicantVendorStaIfaceSerializeOnCtrlEventInd(in_ifaceName, in_eventStr, data)) {
        ALOGE("[Fail] Serializing Vendor StaIface Event <onCtrlEvent>");
        return ndk::ScopedAStatus::fail(SupplicantStatusCode::FAILURE_ARGS_INVALID);
    }

    ADD_INT32_TO_VECTOR(ifId, data);

    return SupplicantSendEvent(SUPPLICANT_VENDOR_STA_IFACE_ON_CTRL_EVENT_IND, data);
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
