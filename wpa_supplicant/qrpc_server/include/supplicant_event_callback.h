/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_EVENT_CALLBACK_H
#define SUPPLICANT_EVENT_CALLBACK_H

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>
#include <rpc/message/wpa_supplicant_vendor/supplicant_vendor_message_def.h>

#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>
#ifdef CONFIG_USE_NONSTD_CALLBACK
#define NON_STANDARD_CERT_GET_BLOB_IND                  (uint16_t)(0x8400 + NON_STANDARD_CERT_GET_BLOB_REQ)
#define NON_STANDARD_CERT_LIST_ALIASES_IND              (uint16_t)(0x8400 + NON_STANDARD_CERT_LIST_ALIASES_REQ)
#endif
extern std::vector<uint16_t> SupplicantEvent;

ndk::ScopedAStatus SupplicantSendEvent(uint16_t eventId, std::vector<uint8_t>& data);

bool someip_send_event(uint16_t method_id, std::vector<uint8_t> &data);

#endif /* SUPPLICANT_EVENT_CALLBACK_H */
