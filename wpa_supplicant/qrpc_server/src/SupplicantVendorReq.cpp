/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant_vendor/supplicant_vendor_msg.h>

#include "SupplicantVendorReq.h"

#include "aidl_manager.h"
#include "supplicant_vendor.h"

using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;


#define SUPPLICANT_VENDOR_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Supplicant Vendor Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool SupplicantVendorSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return VendorSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool SupplicantVendorMsgHandlerListVendorInterfaces(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Vendor Req <ListVendorInterfaces>: (%s)");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<SupplicantVendor> supplicant_vendor_instance;
    if(aidl_manager->getSupplicantVendorInstance(&supplicant_vendor_instance)){
        ALOGE("[Fail] No Supplicant Vendor instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantVendorSerializeStatus(status, outData);
    }

    std::vector<IVendorIfaceInfo> vendorIfaceInfo;
    ndk::ScopedAStatus status = supplicant_vendor_instance->listVendorInterfaces(&vendorIfaceInfo);
    SUPPLICANT_VENDOR_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <ListVendorInterfaces> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantVendorSerializeListVendorInterfacesCfm(param, vendorIfaceInfo, outData);
}
