/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "hostapd_event_callback.h"
using aidl::android::hardware::wifi::hostapd::HostapdStatusCode;

std::vector<uint16_t> HostapdEventArray{
    /* IHostapdCallback */
    HOSTAPD_ON_AP_INSTANCE_INFO_CHANGED_IND,
    HOSTAPD_ON_CONNECTED_CLIENTS_CHANGED_IND,
    HOSTAPD_ON_FAILURE_IND,
    /* IHostapdVendorCallback */
    HOSTAPD_VENDOR_ON_CTRL_EVENT_IND,
    HOSTAPD_VENDOR_ON_AP_INSTANCE_INFO_CHANGED_IND,
    HOSTAPD_VENDOR_ON_FAILURE_IND,
};

ndk::ScopedAStatus HostapdSendEvent(uint16_t eventId, std::vector<uint8_t>& data)
{
    if (!someip_send_event(eventId, data)) {
        ALOGE("Failed to send event ID: 0x%04X", eventId);
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_UNKNOWN);
    }
    return ndk::ScopedAStatus::ok();
}
