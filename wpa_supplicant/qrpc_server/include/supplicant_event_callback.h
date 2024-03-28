/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_EVENT_CALLBACK_H
#define SUPPLICANT_EVENT_CALLBACK_H

#include <android-base/logging.h>

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>

#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

/*Supported events for Phase 1*/
#define SUPPLICANT_EVENT_COUNTS    11

extern uint16_t SupplicantEventArray[SUPPLICANT_EVENT_COUNTS];

ndk::ScopedAStatus SupplicantSendEvent(uint16_t eventId, std::vector<uint8_t>& data);

#endif /* SUPPLICANT_EVENT_CALLBACK_H */
