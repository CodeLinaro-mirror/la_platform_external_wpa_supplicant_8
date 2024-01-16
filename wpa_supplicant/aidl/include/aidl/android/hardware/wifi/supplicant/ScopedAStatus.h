/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SCOPEDSTATUS_H
#define SCOPEDSTATUS_H

#include <aidl/android/hardware/wifi/supplicant/SupplicantStatusCode.h>

namespace ndk {
using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;
using aidl::android::hardware::wifi::supplicant::SupplicantStatus;
class ScopedAStatus : public SupplicantStatus {
public:
    ScopedAStatus() : SupplicantStatus() {}
    ScopedAStatus(SupplicantStatusCode code) : SupplicantStatus(code) {}
    ScopedAStatus(const SupplicantStatusCode code, const std::string &desc)
        : SupplicantStatus(code, desc) {}
    ScopedAStatus(int32_t code, const char* desc)
        : SupplicantStatus(SupplicantStatusCode(code), desc) {}

    static ScopedAStatus ok() {
        return ScopedAStatus();
    }

    static ScopedAStatus fail(SupplicantStatusCode failCode) {
        return ScopedAStatus(failCode);
    }

    bool isOk() {
        return code == SupplicantStatusCode::SUCCESS;
    }

    static ScopedAStatus fromServiceSpecificError(int32_t code) {
        return ScopedAStatus(code, "");
    }

    static ScopedAStatus fromServiceSpecificErrorWithMessage(
        int32_t code, const char* description) {
        return ScopedAStatus(code, description);
    }

    SupplicantStatusCode getServiceSpecificError() {
        return code;
    }
};
} /* ndk */

/* Alias name from auto generated paramter structure */
using binder_status_t = ndk::ScopedAStatus;

/* Create default SupplicantStatus Success state */
#define STATUS_OK (ndk::ScopedAStatus::ok())

#endif /* SCOPEDSTATUS_H */
