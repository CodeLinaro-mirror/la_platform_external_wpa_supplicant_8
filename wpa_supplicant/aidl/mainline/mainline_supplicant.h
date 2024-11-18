/*
 * WPA Supplicant - Mainline supplicant AIDL implementation
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MAINLINE_SUPPLICANT_IMPL_H
#define MAINLINE_SUPPLICANT_IMPL_H

#include <aidl/android/system/wifi/mainline_supplicant/BnMainlineSupplicant.h>

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "utils/wpa_debug.h"
#include "wpa_supplicant_i.h"
}

using ::aidl::android::system::wifi::mainline_supplicant::BnMainlineSupplicant;

class MainlineSupplicant : public BnMainlineSupplicant {
    public:
        MainlineSupplicant(struct wpa_global* global);
        ndk::ScopedAStatus terminate();

    private:
        // Raw pointer to the global structure maintained by the core
        struct wpa_global* wpa_global_;
};

#endif  // MAINLINE_SUPPLICANT_IMPL_H
