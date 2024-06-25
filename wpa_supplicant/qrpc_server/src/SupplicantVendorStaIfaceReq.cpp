/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant_vendor/supplicant_vendor_sta_iface_msg.h>

#include "SupplicantVendorStaIfaceReq.h"

#include "aidl_manager.h"
#include "vendorsta_iface.h"

using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::android::hardware::wifi::supplicant::SupplicantStatusCode;


#define SUPPLICANT_VENDOR_STAIFACE_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Supplicant Vendor Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool VendorStaIfaceSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return VendorSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool VendorStaIfaceMsgHandlerDoDriverCmd(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return VendorStaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Vendor Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return VendorStaIfaceSerializeStatus(status, outData);
    }

    std::string cmd;
    if (!SupplicantVendorStaIfaceParseDoDriverCmdReq(data, length - 4, cmd)) {
        ALOGE("[Fail] Parsing Vendor StaIface Req <DoDriverCmd>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return VendorStaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing Vendor StaIface Req <DoDriverCmd>: (%d)  for iface --> %s", cmd.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantVendorStaIface> vendor_staiface_instance;
    if(aidl_manager->getVendorStaIfaceAidlObjectByIfname(ifName, &vendor_staiface_instance)) {
        ALOGE("[Fail] Vendor Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return VendorStaIfaceSerializeStatus(status, outData);
    }

    std::string res;
    ndk::ScopedAStatus status = vendor_staiface_instance->doDriverCmd(cmd, &res);
    SUPPLICANT_VENDOR_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <DoDriverCmd> resp: (%s)", res.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantVendorStaIfaceSerializeDoDriverCmdCfm(param, res, outData);
}
