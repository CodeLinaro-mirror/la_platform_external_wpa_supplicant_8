/*
 * WPA Supplicant - Shared utility functions and constants
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <android-base/file.h>
#include <fcntl.h>

extern "C"
{
#include "utils/common.h"
}

constexpr char kIfaceDriverName[] = "nl80211";
constexpr mode_t kConfigFileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

/**
 * Ensure that the config file at |config_file_path| exists.
 * Returns 0 on success, or errno otherwise.
 */
int ensureConfigFileExistsAtPath(const std::string& config_file_path);

#endif // SHARED_UTILS_H
