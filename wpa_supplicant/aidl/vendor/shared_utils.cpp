/*
 * WPA Supplicant - Shared utility functions and constants
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "aidl/shared/shared_utils.h"

int ensureConfigFileExistsAtPath(const std::string& config_file_path) {
    int ret = access(config_file_path.c_str(), R_OK);
    if (ret == 0) {
        return 0;
    }
    if (errno == EACCES) {
        ret = chmod(config_file_path.c_str(), kConfigFileMode);
        if (ret == 0) {
            return 0;
        } else {
            wpa_printf(
                MSG_ERROR, "Cannot set RW to %s. Errno: %s",
                config_file_path.c_str(), strerror(errno));
        }
    } else if (errno != ENOENT) {
        wpa_printf(
            MSG_ERROR, "Cannot access %s. Errno: %s",
            config_file_path.c_str(), strerror(errno));
    }
    return errno;
}
