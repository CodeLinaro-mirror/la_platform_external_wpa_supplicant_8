/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_SOMEIP_SERVER_H
#define SUPPLICANT_SOMEIP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

bool SupplicantSomeIPServerInit();
bool SupplicantSomeIPServerStart();
void SupplicantSomeIPServerStop();
void SupplicantSomeIPServerDeinit();

void someip_process_queued_msg();

#ifdef __cplusplus
}
#endif

#endif /* SUPPLICANT_SOMEIP_SERVER_H */
