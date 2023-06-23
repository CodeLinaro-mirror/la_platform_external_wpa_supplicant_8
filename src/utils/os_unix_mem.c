/*
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <string.h>

void * os_memcpy(void *dest, const void *src, size_t n)
{
	if (n)
		memcpy(dest, src, n);
	return dest;
}

void * os_memmove(void *dest, const void *src, size_t n)
{
	if (n)
		memmove(dest, src, n);
	return dest;
}

void * os_memset(void *s, int c, size_t n)
{
	if (n)
		memset(s, c, n);
	return s;
}

int os_memcmp(const void *s1, const void *s2, size_t n)
{
	if (n)
		return memcmp(s1, s2, n);
	return 0;
}
