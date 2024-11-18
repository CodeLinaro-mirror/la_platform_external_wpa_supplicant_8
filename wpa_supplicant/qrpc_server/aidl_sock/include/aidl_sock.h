/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef AIDL_SOCK_H
#define AIDL_SOCK_H

#ifdef __cplusplus
extern "C" {
#endif

void qti_create_aidl_socket(int *fd, const char *path);
void qti_destroy_aidl_socket();
bool qti_connect_aidl_socket(const char *path, void (*handler)(void));
void qti_disconnect_aidl_socket();
void qti_notify_aidl_socket();
void qti_aidl_sock_handler(int sock, void *eloop_ctx, void *sock_ctx);

#ifdef __cplusplus
}
#endif

#endif
