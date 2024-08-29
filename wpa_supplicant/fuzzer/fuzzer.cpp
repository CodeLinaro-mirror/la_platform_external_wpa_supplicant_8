/*
 ** Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 ** SPDX-License-Identifier: BSD-3-Clause-Clear
 **/
#include <fuzzbinder/libbinder_ndk_driver.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <android/binder_interface_utils.h>
#include "supplicant_vendor.h"

using aidl::vendor::qti::hardware::wifi::supplicant::SupplicantVendor;
std::shared_ptr<SupplicantVendor> service;

#ifdef CONFIG_AIDL_FUZZING
extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv) {
    struct wpa_params params;
    struct wpa_global *global;

    memset(&params, 0, sizeof(params));
    params.wpa_debug_level = MSG_INFO;

    global = wpa_supplicant_init(&params);
    if (global == NULL)
        return -1;
    service = ndk::SharedRefBase::make<SupplicantVendor>(global);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (service == nullptr) {
        return -1;
    }

    android::fuzzService(service->asBinder().get(), FuzzedDataProvider(data, size));
    return 0;
}
#endif

