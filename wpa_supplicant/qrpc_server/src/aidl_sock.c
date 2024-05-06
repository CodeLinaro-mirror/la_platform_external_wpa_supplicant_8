/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 *
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name(s) of the above-listed copyright holder(s) nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "includes.h"
#include <sys/un.h>
#include <sys/stat.h>
#include <grp.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/list.h"

#include "aidl_sock.h"
#include "aidl_i.h"
#include "supplicant_message_handler.h"

static const char* spath = "/data/vendor/wifi/wpa/aidl_server";
static const char* cpath = "/data/vendor/wifi/wpa/aidl_client";
int server_sockect_fd = -1;
int client_sockect_fd = -1;
uint16_t method_id_buf = 0;
uint16_t method_id_count = 0;

void wpas_create_aidl_socket(int *fd)
{
	wpa_printf(MSG_DEBUG, "Creating aidl socket");

	struct sockaddr_un addr;
	int flags;

	*fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (*fd < 0) {
		wpa_printf(MSG_ERROR, "aidl socket: %s", strerror(errno));
		goto fail;
	}

	os_memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_UNIX;
	os_strlcpy(addr.sun_path, spath, sizeof(addr.sun_path));
	if (bind(*fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		wpa_printf(MSG_ERROR, "aidl sockect <bind> failed: %s", strerror(errno));
		if (connect(*fd, (struct sockaddr *) &addr,
			    sizeof(addr)) < 0) {
			wpa_printf(MSG_INFO, "aidl sockect exists, but does not"
				   " allow connections - assuming it was left"
				   "over from forced program termination");
			if (unlink(spath) < 0) {
				wpa_printf(MSG_ERROR,
					   "Could not unlink existing aidl sockect '%s': %s",
					   spath, strerror(errno));
				goto fail;
			}
			if (bind(*fd, (struct sockaddr *) &addr,
				 sizeof(addr)) < 0) {
				wpa_printf(MSG_ERROR, "aidl socket <re-bind> failed: %s",
					   strerror(errno));
				goto fail;
			}
			wpa_printf(MSG_INFO, "Successfully replaced leftover "
				   "aidl sockect '%s'", spath);
		} else {
			wpa_printf(MSG_ERROR, "aidl sockect exists and seems to "
				   "be in use - cannot override it");
			wpa_printf(MSG_ERROR, "Delete '%s' manually if it is "
				   "not used anymore", spath);
			goto fail;
		}
	}

	if (chmod(spath, S_IRWXU | S_IRWXG) < 0) {
		wpa_printf(MSG_ERROR, "chmod[ctrl_interface=%s]: %s",
			   spath, strerror(errno));
		goto fail;
	}

	flags = fcntl(*fd, F_GETFL);
	if (flags >= 0) {
		flags |= O_NONBLOCK;
		if (fcntl(*fd, F_SETFL, flags) < 0) {
			wpa_printf(MSG_INFO, "fcntl(ctrl, O_NONBLOCK): %s",
				   strerror(errno));
			/* Not fatal, continue on.*/
		}
	}
	wpa_printf(MSG_DEBUG, "Aidl sockect created");
	server_sockect_fd = *fd;
	return;
fail:
	if (*fd >= 0) {
		close(*fd);
		*fd = -1;
	}
	if (spath) {
		unlink(spath);
	}
	wpa_printf(MSG_ERROR, "Cannot create aidl sockect");
}

void wpas_destroy_aidl_socket()
{
	unlink(spath);
	if (server_sockect_fd > -1) {
		close(server_sockect_fd);
		server_sockect_fd = -1;
	}
	wpa_printf(MSG_DEBUG, "Aidl sockect destroyed");
}

bool wpas_connect_aidl_socket()
{
	wpa_printf(MSG_DEBUG, "Connecting to aidl socket");

	struct sockaddr_un saddr;
	struct sockaddr_un caddr;

	static int counter = 0;
	int ret;
	size_t res;
	int tries = 0;
	int flags;

	client_sockect_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (client_sockect_fd < 0) {
		wpa_printf(MSG_ERROR, "aidl socket: %s", strerror(errno));
		return false;
	}

	caddr.sun_family = AF_UNIX;
	counter++;

try_again:
	os_strlcpy(caddr.sun_path, cpath, sizeof(caddr.sun_path));

	tries++;
	if (bind(client_sockect_fd, (struct sockaddr *) &caddr, sizeof(caddr)) < 0) {
		if (errno == EADDRINUSE && tries < 2) {
			wpa_printf(MSG_ERROR, "aidl sockect <bind> failed: %s, retrying", strerror(errno));
			unlink(caddr.sun_path);
			goto try_again;
		}
		close(client_sockect_fd);
		wpa_printf(MSG_ERROR, "aidl sockect <re-bind> failed: %s", strerror(errno));
		return false;
	}

	saddr.sun_family = AF_UNIX;
	os_strlcpy(saddr.sun_path, spath, sizeof(saddr.sun_path));

	if (connect(client_sockect_fd, (struct sockaddr *) &saddr,
			sizeof(saddr)) < 0) {
		wpa_printf(MSG_ERROR, "aidl sockect <connect> failed: %s", strerror(errno));
		close(client_sockect_fd);
		unlink(caddr.sun_path);
		return false;
	}

	/*
	 * Make socket non-blocking so that we don't hang forever if
	 * target dies unexpectedly.
	 */
	flags = fcntl(client_sockect_fd, F_GETFL);
	if (flags >= 0) {
		flags |= O_NONBLOCK;
		if (fcntl(client_sockect_fd, F_SETFL, flags) < 0) {
			wpa_printf(MSG_INFO, "fcntl(ctrl, O_NONBLOCK): %s",
				   strerror(errno));
			/* Not fatal, continue on.*/
		}
	}
	wpa_printf(MSG_DEBUG, "Aidl sockect connected");
	return true;
}


void wpas_disconnect_aidl_socket()
{
	unlink(cpath);
	if (client_sockect_fd >= 0){
		close(client_sockect_fd);
		client_sockect_fd = -1;
	}
}

void wpas_sendto_aidl_socket(uint16_t message_type, uint8_t *data, size_t length)
{
	wpa_printf(MSG_DEBUG, "Sending to Aidl sockect, 0x%04X", message_type);

	method_id_buf = message_type;
	method_id_count++;

	struct os_reltime started_at;
	int res;

	errno = 0;
	started_at.sec = 0;
	started_at.usec = 0;
retry_send:
	if (send(client_sockect_fd, data, length, 0) < 0) {
		wpa_printf(MSG_DEBUG, "Initial Sending failed, %s", strerror(errno));
		if (errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK)
		{
			/*
			 * Must be a non-blocking socket... Try for a bit
			 * longer before giving up.
			 */
			if (started_at.sec == 0)
				os_get_reltime(&started_at);
			else {
				struct os_reltime n;
				os_get_reltime(&n);
				/* Try for a few seconds. */
				if (os_reltime_expired(&n, &started_at, 5))
					goto send_err;
			}
			os_sleep(1, 0);
			wpa_printf(MSG_DEBUG, "Retrying send ...");
			goto retry_send;
		}
	send_err:
		wpa_printf(MSG_DEBUG, "Sending failed, %s", strerror(errno));
		return;
	}
	wpa_printf(MSG_DEBUG, "Message sent");
}

void wpas_aidl_sock_handler(int sock, void *eloop_ctx, void *sock_ctx)
{
	struct wpa_global *global = eloop_ctx;
	struct wpas_aidl_priv *priv = sock_ctx;
	uint8_t *buf;
	size_t res;
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);

	buf = os_malloc(8192);
	if (!buf)
		return;
	res = recvfrom(sock, buf, 8192, 0,
			   (struct sockaddr *) &from, &fromlen);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "recvfrom(ctrl_iface): %s",
			   strerror(errno));
		os_free(buf);
		return;
	}
	if ((size_t) res > 8192) {
		wpa_printf(MSG_ERROR, "recvform(ctrl_iface): input truncated");
		os_free(buf);
		return;
	}

	wpa_printf(MSG_DEBUG, "Message received");

	while(method_id_count == 1)
	{
		SupplicantProcessSomeIPRequestMessage(method_id_buf, buf, res);
		method_id_count--;
	}

	os_memset(buf, 0, res);
	os_free(buf);
}
