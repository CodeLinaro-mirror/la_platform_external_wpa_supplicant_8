/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef HOSTAPD_SOMEIP_SERVER_H
#define HOSTAPD_SOMEIP_SERVER_H


#ifdef __cplusplus
extern "C" {
#endif

bool HostapdSomeIPServerInit();
bool HostapdSomeIPServerStart();
void HostapdSomeIPServerStop();
void HostapdSomeIPServerDeinit();
void someip_process_queued_msg();

#ifdef __cplusplus
}
#endif

#endif /* HOSTAPD_SOMEIP_SERVER_H */
