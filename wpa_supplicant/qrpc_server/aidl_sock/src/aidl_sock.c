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
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include <rpc/util/log_common.h>

#include "aidl_sock.h"

static const char *spath;
static const char *cpath;
static void (*sock_handler)(void);

int server_sockect_fd = -1;
int client_sockect_fd = -1;

struct reltime {
	long sec;
	long usec;
};

int get_reltime(struct reltime *t)
{
	static clockid_t clock_id = CLOCK_REALTIME;
	struct timespec ts;
	int res;

	while (1) {
		res = clock_gettime(clock_id, &ts);
		if (res == 0) {
			t->sec = ts.tv_sec;
			t->usec = ts.tv_nsec / 1000;
			return 0;
		}
		switch (clock_id) {
			case CLOCK_REALTIME:
				return -1;
		}
	}
}

static inline void reltime_sub(struct reltime *a, struct reltime *b,
				  struct reltime *res)
{
	res->sec = a->sec - b->sec;
	res->usec = a->usec - b->usec;
	if (res->usec < 0) {
		res->sec--;
		res->usec += 1000000;
	}
}

static inline int reltime_expired(struct reltime *now,
				     struct reltime *ts,
				     long timeout_secs)
{
	struct reltime age;

	reltime_sub(now, ts, &age);
	return (age.sec > timeout_secs) ||
	       (age.sec == timeout_secs && age.usec > 0);
}

void qti_create_aidl_socket(int *fd, const char *path)
{
	spath = path;
	ALOGE("Creating aidl socket");

	struct sockaddr_un addr;
	int flags;

	*fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (*fd < 0) {
		ALOGE("Aidl socket: %s", strerror(errno));
		goto fail;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strlcpy(addr.sun_path, spath, sizeof(addr.sun_path));
	if (bind(*fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		ALOGE("Aidl server sockect <bind> failed: %s", strerror(errno));
		if (connect(*fd, (struct sockaddr *) &addr,
			    sizeof(addr)) < 0) {
			ALOGI("Aidl sockect exists, but does not"
				   " allow connections - assuming it was left"
				   "over from forced program termination");
			if (unlink(spath) < 0) {
				ALOGE("Could not unlink existing aidl sockect '%s': %s",
					   spath, strerror(errno));
				goto fail;
			}
			if (bind(*fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
				ALOGE("Aidl socket <re-bind> failed: %s", strerror(errno));
				goto fail;
			}
			ALOGI("Successfully replaced leftover aidl sockect '%s'", spath);
		} else {
			ALOGE("Aidl sockect exists and seems to be in use - cannot override it");
			ALOGE("Delete '%s' manually if it is not used anymore", spath);
			goto fail;
		}
	}

	if (chmod(spath, S_IRWXU | S_IRWXG) < 0) {
		ALOGE("chmod[ctrl_interface=%s]: %s", spath, strerror(errno));
		goto fail;
	}

	flags = fcntl(*fd, F_GETFL);
	if (flags >= 0) {
		flags |= O_NONBLOCK;
		if (fcntl(*fd, F_SETFL, flags) < 0) {
			ALOGI("fcntl(ctrl, O_NONBLOCK): %s", strerror(errno));
			/* Not fatal, continue on.*/
		}
	}
	ALOGD("Aidl sockect created");
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
	ALOGE("Cannot create aidl sockect");
}

void qti_destroy_aidl_socket()
{
	unlink(spath);
	if (server_sockect_fd > -1) {
		close(server_sockect_fd);
		server_sockect_fd = -1;
	}
	ALOGD("Aidl sockect destroyed");
}

bool qti_connect_aidl_socket(const char *path, void (*handler)(void))
{
	cpath = path;
	sock_handler = handler;
	ALOGD("Connecting to aidl socket");

	struct sockaddr_un saddr;
	struct sockaddr_un caddr;

	int tries = 0;
	int flags;

	client_sockect_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (client_sockect_fd < 0) {
		ALOGE("Aidl socket: %s", strerror(errno));
		return false;
	}

	caddr.sun_family = AF_UNIX;

try_again:
	strlcpy(caddr.sun_path, cpath, sizeof(caddr.sun_path));

	tries++;
	if (bind(client_sockect_fd, (struct sockaddr *) &caddr, sizeof(caddr)) < 0) {
		if (errno == EADDRINUSE && tries < 2) {
			ALOGE("Aidl client sockect <bind> failed: %s, retrying", strerror(errno));
			unlink(caddr.sun_path);
			goto try_again;
		}
		close(client_sockect_fd);
		ALOGE("Aidl sockect <re-bind> failed: %s", strerror(errno));
		return false;
	}

	saddr.sun_family = AF_UNIX;
	strlcpy(saddr.sun_path, spath, sizeof(saddr.sun_path));

	if (connect(client_sockect_fd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		ALOGE("Aidl sockect <connect> failed: %s", strerror(errno));
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
			ALOGI("fcntl(ctrl, O_NONBLOCK): %s", strerror(errno));
			/* Not fatal, continue on.*/
		}
	}
	ALOGD("Aidl socket connected");
	return true;
}


void qti_disconnect_aidl_socket()
{
	unlink(cpath);
	if (client_sockect_fd >= 0){
		close(client_sockect_fd);
		client_sockect_fd = -1;
	}
	ALOGD("Aidl sockect disconnected");
}

void qti_notify_aidl_socket()
{
	ALOGD("Notifying aidl sockect");

	struct reltime started_at;

	char flag[] = {0x01};

	errno = 0;
	started_at.sec = 0;
	started_at.usec = 0;
retry_send:
	if (send(client_sockect_fd, flag, 1, 0) < 0) {
		ALOGD("Initial notify failed, %s", strerror(errno));
		if (errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK)
		{
			/*
			 * Must be a non-blocking socket... Try for a bit
			 * longer before giving up.
			 */
			if (started_at.sec == 0)
				get_reltime(&started_at);
			else {
				struct reltime n;
				get_reltime(&n);
				/* Try for a few seconds. */
				if (reltime_expired(&n, &started_at, 5))
					goto send_err;
			}
			sleep(1);
			ALOGD("Re-notifying...");
			goto retry_send;
		}
	send_err:
		ALOGD("Fail to notify, %s", strerror(errno));
		return;
	}
	ALOGD("Aidl socket notified");
}

void qti_aidl_sock_handler(int sock, void *eloop_ctx, void *sock_ctx)
{
	uint8_t *buf;
	size_t res;
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);

	buf = malloc(1);
	if (!buf)
		return;
	res = recvfrom(sock, buf, 1, 0, (struct sockaddr *) &from, &fromlen);
	if (res < 0) {
		ALOGE("recvfrom(ctrl_iface): %s", strerror(errno));
		free(buf);
		return;
	}
	if ((size_t) res > 1) {
		ALOGE("Notification error: recv(%ld) too long", res);
		free(buf);
		return;
	}

	ALOGD("Message received");
	sock_handler();

	memset(buf, 0, res);
	free(buf);
}
