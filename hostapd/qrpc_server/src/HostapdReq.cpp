/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/hostapd/hostapd_msg.h>
#include "hostapd_someip_server.h"
#include "HostapdReq.h"
#include "hostapd.h"
#include "aidl_helper.h"

using namespace aidl::android::hardware::wifi::hostapd;
static std::shared_ptr<Hostapd> hostapd_object_;

#define HOSTAPD_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Hostapd Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool HostapdSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return HostapdSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool HostapdMsgHandlerAddAccessPoint(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    AddAccessPointReqParam buffer;
    if (!HostapdParseAddAccessPointReq(data, length, buffer)) {
        ALOGE("[Fail] Parsing Hostapd Req <AddAccessPoint>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdSerializeStatus(status, outData);
    }
    hostapd_object_ = getHostapdObject();
    if(!hostapd_object_){
        ALOGE("[Fail] Get Hostapd Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdSerializeStatus(status, outData);
    }
    ndk::ScopedAStatus status = hostapd_object_->addAccessPoint(buffer.ifaceParams,buffer.nwParams);
    HOSTAPD_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <AddAccessPoint> resp:");
    HostapdSerializeStatus(status, outData);
    return HostapdSerializeAddAccessPointCfm(outData);
}

bool HostapdMsgHandlerForceClientDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ForceClientDisconnectReqParam buffer;
    if (!HostapdParseForceClientDisconnectReq(data, length, buffer)) {
        ALOGE("[Fail] Parsing Hostapd Req <ForceClientDisconnect>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdSerializeStatus(status, outData);
    }
    hostapd_object_ = getHostapdObject();
    if(!hostapd_object_){
        ALOGE("[Fail] Get Hostapd Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdSerializeStatus(status, outData);
    }
    ndk::ScopedAStatus status = hostapd_object_->forceClientDisconnect(buffer.ifaceName,buffer.clientAddress,buffer.reasonCode);
    HOSTAPD_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <ForceClientDisconnect> resp:");
    HostapdSerializeStatus(status, outData);
    return HostapdSerializeForceClientDisconnectCfm(outData);
}

bool HostapdMsgHandlerRemoveAccessPoint(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    std::string buffer;
    if (!HostapdParseRemoveAccessPointReq(data, length, buffer)) {
        ALOGE("[Fail] Parsing Hostapd Req <RemoveAccessPoint>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdSerializeStatus(status, outData);
    }
    hostapd_object_ = getHostapdObject();
    if(!hostapd_object_){
        ALOGE("[Fail] Get Hostapd Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdSerializeStatus(status, outData);
    }
    ndk::ScopedAStatus status = hostapd_object_->removeAccessPoint(buffer);
    HOSTAPD_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <RemoveAccessPoint> resp");
    HostapdSerializeStatus(status, outData);
    return HostapdSerializeRemoveAccessPointCfm(outData);
}

bool HostapdMsgHandlerSetDebugParams(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    DebugLevel buff;
    if (!HostapdParseSetDebugParamsReq(data, length, buff)) {
        ALOGE("[Fail] Parsing Hostapd Req <SetDebugParams>");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_ARGS_INVALID);
        return HostapdSerializeStatus(status, outData);
    }
    hostapd_object_ = getHostapdObject();
    if(!hostapd_object_){
        ALOGE("[Fail] Get Hostapd Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdSerializeStatus(status, outData);
    }
    ndk::ScopedAStatus status = hostapd_object_->setDebugParams(buff);
    HOSTAPD_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <SetDebugParams> resp");
    HostapdSerializeStatus(status, outData);
    return HostapdSerializeSetDebugParamsCfm(outData);
}

bool HostapdMsgHandlerTerminate(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Hostapd Req <Terminate>");
    hostapd_object_ = getHostapdObject();
    if(!hostapd_object_){
        ALOGE("[Fail] Get Hostapd Object Failed");
        ndk::ScopedAStatus status(HostapdStatusCode::FAILURE_UNKNOWN);
        return HostapdSerializeStatus(status, outData);
    }
    ndk::ScopedAStatus status = hostapd_object_->terminate();
    HOSTAPD_PRINT_CFM_STATUS(__func__, status);
    ALOGI("Sending <Terminate> resp");
    HostapdSerializeStatus(status, outData);
    return HostapdSerializeTerminateCfm(outData);
}
