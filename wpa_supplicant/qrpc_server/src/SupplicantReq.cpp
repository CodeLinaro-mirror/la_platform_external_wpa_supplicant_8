/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <android-base/logging.h>

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/message/wpa_supplicant/supplicant_msg.h>

#include "SupplicantReq.h"

#include "aidl_manager.h"
#include "supplicant.h"
#include "sta_iface.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_PRINT_CFM_STATUS(function, status) \
    do { \
        LOG(INFO) << "Supplicant Someip Service: " << (function) \
                   << " return status: " << toString((status).code); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool SupplicantSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return SupplicantSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool SupplicantMsgHandlerAddStaInterface(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    std::string ifName;
    if (!SupplicantParseAddStaInterfaceReq(data, length, ifName)) {
        LOG(ERROR) << "[Fail] Parsing Supplicant Req <AddStaInterface>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing Supplicant Req <AddStaInterface>: (" << ifName <<")";

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        LOG(ERROR) << "[Fail] No Supplicant instance";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    std::shared_ptr<ISupplicantStaIface> staIface;
    ndk::ScopedAStatus status = supplicant_instance->addStaInterface(ifName, &staIface);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    int32_t instanceId;
    if (status.isOk() && staIface != nullptr)
        instanceId = std::dynamic_pointer_cast<StaIface>(staIface)->getIfaceInstanceId();
    LOG(INFO) << "Sending <AddStaInterface> resp: (" << std::hex << instanceId << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeAddStaInterfaceCfm(param, instanceId, outData);
}

bool SupplicantMsgHandlerRemoveInterface(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    IfaceInfo ifaceInfo;
    if (!SupplicantParseRemoveInterfaceReq(data, length, ifaceInfo)) {
        LOG(ERROR) << "[Fail] Parsing Supplicant Req <RemoveInterface>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing Supplicant Req <RemoveInterface>: (" << ifaceInfo.name << ")";

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        LOG(ERROR) << "[Fail] No Supplicant instance";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->removeInterface(ifaceInfo);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerSetDebugParams(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    SetDebugParamsReqParam buff;
    if (!SupplicantParseSetDebugParamsReq(data, length, buff)) {
        LOG(ERROR) << "[Fail] Parsing Supplicant Req <SetDebugParams>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }
    LOG(INFO) << "Processing Supplicant Req <SetDebugParams>: (" << toString(buff.level) << "), (" << buff.showTimestamp << "), (" << buff.showKeys << ")"; 

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        LOG(ERROR) << "[Fail] No Supplicant instance";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->setDebugParams(buff.level, buff.showTimestamp, buff.showKeys);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerSetConcurrencyPriority(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    IfaceType ifaceType;
    if (!SupplicantParseSetConcurrencyPriorityReq(data, length, ifaceType)) {
        LOG(ERROR) << "[Fail] Parsing Supplicant Req <SetConcurrencyPriority>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }
    LOG(INFO) << "Processing Supplicant Req <SetConcurrencyPriority>: (" << toString(ifaceType) << ")"; 

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        LOG(ERROR) << "[Fail] No Supplicant instance";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->setConcurrencyPriority(ifaceType);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerTerminate(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    LOG(INFO) << "Processing Supplicant Req <Terminate>";

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        LOG(ERROR) << "[Fail] No Supplicant instance";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->terminate();
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}
