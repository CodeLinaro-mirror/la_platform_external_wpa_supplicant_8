/*
 * WPA Supplicant - Mainline supplicant AIDL implementation
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "mainline_supplicant.h"

using ::ndk::ScopedAStatus;

MainlineSupplicant::MainlineSupplicant(struct wpa_global* global) {
    wpa_global_ = global;
}

ndk::ScopedAStatus MainlineSupplicant::terminate() {
    wpa_printf(MSG_INFO, "Terminating...");
    wpa_supplicant_terminate_proc(wpa_global_);
    return ndk::ScopedAStatus::ok();
}
