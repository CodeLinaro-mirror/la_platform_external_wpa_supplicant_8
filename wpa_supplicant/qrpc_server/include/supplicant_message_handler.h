/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_MESSAGE_HANDLER_H
#define SUPPLICANT_MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

void SupplicantProcessSomeIPRequestMessage(uint16_t methodId, uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif


#endif /* SUPPLICANT_MESSAGE_HANDLER_H */
