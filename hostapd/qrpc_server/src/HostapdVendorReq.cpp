/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/hostapd_vendor/hostapd_vendor_msg.h>
#include "hostapd_someip_server.h"
#include "HostapdVendorReq.h"
#include "hostapd_vendor.h"
#include "aidl_helper.h"

using namespace aidl::vendor::qti::hardware::wifi::hostapd;
using aidl::android::hardware::wifi::hostapd::HostapdStatusCode;

static std::shared_ptr<HostapdVendor> hostapd_vendor_object_;

#define HOSTAPD_VENDOR_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Hostapd Vendor Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool HostapdVendorSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return VendorSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool HostapdVendorMsgHandlerListVendorInterfaces(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!HostapdVendorParseListVendorInterfacesReq(data, length)) {
        ALOGE("[Fail] Parsing Hostapd Vendor Req <ListVendorInterfaces>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdVendorSerializeStatus(status, outData);
    }
    hostapd_vendor_object_ = getHostapdVendorObject();
    if(!hostapd_vendor_object_){
        ALOGE("[Fail] Get Hostapd Vendor Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdVendorSerializeStatus(status, outData);
    }
    std::vector<std::string> aidl_return;
    ndk::ScopedAStatus status = hostapd_vendor_object_->listVendorInterfaces(&aidl_return);
    HOSTAPD_VENDOR_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <ListVendorInterfaces> resp");
    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return HostapdVendorSerializeListVendorInterfacesCfm(param,aidl_return,outData); 
}


bool HostapdVendorMsgHandlerDoDriverCmd(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
	DoDriverCmdReqParam buffer;
    if (!HostapdVendorParseDoDriverCmdReq(data, length, buffer)) {
        ALOGE("[Fail] Parsing Hostapd Vendor Req <DoDriverCmd>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdVendorSerializeStatus(status, outData);
    }
    hostapd_vendor_object_ = getHostapdVendorObject();
    if(!hostapd_vendor_object_){
        ALOGE("[Fail] Get Hostapd Vendor Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdVendorSerializeStatus(status, outData);
    }
    std::string aidl_return;
    ndk::ScopedAStatus status = hostapd_vendor_object_->doDriverCmd(buffer.iface, buffer.cmd, &aidl_return);
    HOSTAPD_VENDOR_PRINT_CFM_STATUS(__func__, status);
	ALOGI("Sending <DoDriverCmd> resp");
    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return HostapdVendorSerializeDoDriverCmdCfm(param, aidl_return, outData);
}
