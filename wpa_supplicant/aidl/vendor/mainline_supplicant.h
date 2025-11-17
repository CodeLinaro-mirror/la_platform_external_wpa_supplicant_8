/*
 * WPA Supplicant - Root mainline supplicant interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MAINLINE_SUPPLICANT_H
#define MAINLINE_SUPPLICANT_H

#include <aidl/android/hardware/wifi/supplicant/SupplicantStatusCode.h>

#include <aidl/android/system/wifi/mainline_supplicant/BnMainlineSupplicant.h>

#include "supplicant.h"

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "utils/wpa_debug.h"
#include "wpa_supplicant_i.h"
#include "scan.h"
}

using aidl::android::hardware::wifi::supplicant::ISupplicant;
using aidl::android::hardware::wifi::supplicant::Supplicant;
using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;

using aidl::android::system::wifi::mainline_supplicant::BnMainlineSupplicant;
using aidl::android::system::wifi::mainline_supplicant::ISupplicantNanIface;

class MainlineSupplicant : public BnMainlineSupplicant {
    public:
        MainlineSupplicant(struct wpa_global* global);
        ~MainlineSupplicant() override = default;

        // Aidl methods exposed.
        ::ndk::ScopedAStatus getVendorSupplicant(
                std::shared_ptr<ISupplicant>* _aidl_return) override;
        ::ndk::ScopedAStatus addNanInterface(const std::string& in_ifaceName,
                std::shared_ptr<ISupplicantNanIface>* _aidl_return) override;
        ::ndk::ScopedAStatus removeNanInterface(const std::string& in_ifaceName)
                override;

    private:
        struct wpa_global* wpa_global_;
        std::shared_ptr<Supplicant> vendor_supplicant_;
};

#endif // MAINLINE_SUPPLICANT_H
