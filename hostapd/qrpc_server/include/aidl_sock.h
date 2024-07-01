/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef AIDL_SOCK_H
#define AIDL_SOCK_H

#ifdef __cplusplus
extern "C" {
#endif

void hostapd_create_aidl_socket(int *fd);
void hostapd_destroy_aidl_socket();
bool hostapd_connect_aidl_socket();
void hostapd_disconnect_aidl_socket();
void hostapd_notify_aidl_socket();
void hostapd_aidl_sock_handler(int sock, void *eloop_ctx, void *sock_ctx);

#ifdef __cplusplus
}
#endif

#endif
