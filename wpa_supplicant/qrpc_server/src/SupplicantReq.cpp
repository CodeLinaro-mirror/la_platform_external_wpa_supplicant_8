/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_msg.h>

#include "SupplicantReq.h"

#include "aidl_manager.h"
#include "supplicant.h"
#include "sta_iface.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Supplicant Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
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
        ALOGE("[Fail] Parsing Supplicant Req <AddStaInterface>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }

    ALOGI("Processing Supplicant Req <AddStaInterface>: (%s)", ifName.c_str());

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    std::shared_ptr<ISupplicantStaIface> staIface;
    ndk::ScopedAStatus status = supplicant_instance->addStaInterface(ifName, &staIface);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    int32_t instanceId;
    if (status.isOk() && staIface != nullptr)
        instanceId = std::dynamic_pointer_cast<StaIface>(staIface)->getIfaceInstanceId();
    ALOGI("Sending <AddStaInterface> resp: (%d)", instanceId);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeAddStaInterfaceCfm(param, instanceId, outData);
}

bool SupplicantMsgHandlerGetDebugLevel(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Req <GetDebugLevel>");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    DebugLevel level;
    ndk::ScopedAStatus status = supplicant_instance->getDebugLevel(&level);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetDebugLevel> resp: (%s)", toString(level));

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeGetDebugLevelCfm(param, level, outData);
}

bool SupplicantMsgHandlerIsDebugShowKeysEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Req <IsDebugShowKeysEnabled>");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    bool enable;
    ndk::ScopedAStatus status = supplicant_instance->isDebugShowKeysEnabled(&enable);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <IsDebugShowKeysEnabled> resp: (%d)", enable);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeIsDebugShowKeysEnabledCfm(param, enable, outData);
}

bool SupplicantMsgHandlerIsDebugShowTimestampEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Req <IsDebugShowTimestampEnabled>");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    bool enable;
    ndk::ScopedAStatus status = supplicant_instance->isDebugShowTimestampEnabled(&enable);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <IsDebugShowTimestampEnabled> resp: (%d)", enable);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeIsDebugShowTimestampEnabledCfm(param, enable, outData);
}

bool SupplicantMsgHandlerListInterfaces(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Req <ListInterfaces>");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    std::vector<IfaceInfo> ifaceInfo;
    ndk::ScopedAStatus status = supplicant_instance->listInterfaces(&ifaceInfo);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <ListInterfaces> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantSerializeListInterfacesCfm(param, ifaceInfo, outData);
}

bool SupplicantMsgHandlerRemoveInterface(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    IfaceInfo ifaceInfo;
    if (!SupplicantParseRemoveInterfaceReq(data, length, ifaceInfo)) {
        ALOGE("[Fail] Parsing Supplicant Req <RemoveInterface>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }

    ALOGI("Processing Supplicant Req <RemoveInterface>: (%s)", ifaceInfo.name.c_str());

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->removeInterface(ifaceInfo);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerSetConcurrencyPriority(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    IfaceType ifaceType;
    if (!SupplicantParseSetConcurrencyPriorityReq(data, length, ifaceType)) {
        ALOGE("[Fail] Parsing Supplicant Req <SetConcurrencyPriority>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }
    ALOGI("Processing Supplicant Req <SetConcurrencyPriority>: (%s)", toString(ifaceType).c_str()); 

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->setConcurrencyPriority(ifaceType);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerSetDebugParams(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    SetDebugParamsReqParam buff;
    if (!SupplicantParseSetDebugParamsReq(data, length, buff)) {
        ALOGE("[Fail] Parsing Supplicant Req <SetDebugParams>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return SupplicantSerializeStatus(status, outData);
    }
    ALOGI("Processing Supplicant Req <SetDebugParams>: (%s), (%d), (%d),", 
            toString(buff.level).c_str(), buff.showTimestamp, buff.showKeys); 

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->setDebugParams(buff.level, buff.showTimestamp, buff.showKeys);
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}

bool SupplicantMsgHandlerTerminate(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    ALOGI("Processing Supplicant Req <Terminate>");

    AidlManager *aidl_manager = AidlManager::getInstance();
    std::shared_ptr<Supplicant> supplicant_instance;
    if(aidl_manager->getSupplicantInstance(&supplicant_instance)){
        ALOGE("[Fail] No Supplicant instance");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_UNKNOWN);
        return SupplicantSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = supplicant_instance->terminate();
    SUPPLICANT_PRINT_CFM_STATUS(__func__, status);

    return SupplicantSerializeStatus(status, outData);
}
