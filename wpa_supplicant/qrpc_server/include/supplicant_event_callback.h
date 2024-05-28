/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_EVENT_CALLBACK_H
#define SUPPLICANT_EVENT_CALLBACK_H

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>

#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

extern std::vector<uint16_t> SupplicantEvent;

ndk::ScopedAStatus SupplicantSendEvent(uint16_t eventId, std::vector<uint8_t>& data);

bool someip_send_event(uint16_t message_type, std::vector<uint8_t> &data);

#endif /* SUPPLICANT_EVENT_CALLBACK_H */
