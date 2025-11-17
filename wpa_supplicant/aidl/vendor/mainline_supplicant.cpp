/*
 * WPA Supplicant - Root mainline supplicant interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "mainline_supplicant.h"
#include "misc_utils.h"

using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;

MainlineSupplicant::MainlineSupplicant(struct wpa_global* global)
        : wpa_global_(global) {
    wpa_printf(MSG_INFO, "Creating the mainline supplicant instance");
    vendor_supplicant_ = ndk::SharedRefBase::make<Supplicant>(global);
}

::ndk::ScopedAStatus MainlineSupplicant::getVendorSupplicant(
        std::shared_ptr<ISupplicant>* _aidl_return) {
    wpa_printf(MSG_INFO, "Returning the vendor supplicant object");
    *_aidl_return = vendor_supplicant_;
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus MainlineSupplicant::addNanInterface(
        const std::string& in_ifaceName,
        std::shared_ptr<ISupplicantNanIface>* _aidl_return) {
    wpa_printf(MSG_ERROR, "addNanInterface is not supported");
    return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus MainlineSupplicant::removeNanInterface(
        const std::string& in_ifaceName) {
    wpa_printf(MSG_ERROR, "removeNanInterface is not supported");
    return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}
