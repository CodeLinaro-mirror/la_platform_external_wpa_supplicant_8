/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef HOSTAPD_EVENT_CALLBACK_H
#define HOSTAPD_EVENT_CALLBACK_H

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/hostapd/hostapd_message_def.h>
#include <rpc/message/hostapd_vendor/hostapd_vendor_message_def.h>
#include <aidl/android/hardware/wifi/hostapd/ScopedAStatus.h>

extern std::vector<uint16_t> HostapdEvent;

ndk::ScopedAStatus HostapdSendEvent(uint16_t eventId, std::vector<uint8_t>& data);
bool someip_send_event(uint16_t message_type, std::vector<uint8_t> &data);

#endif /* HOSTAPD_EVENT_CALLBACK_H */
