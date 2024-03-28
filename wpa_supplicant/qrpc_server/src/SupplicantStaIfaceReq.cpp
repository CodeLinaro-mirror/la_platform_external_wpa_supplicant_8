/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <android-base/logging.h>

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/message/wpa_supplicant/supplicant_sta_iface_msg.h>

#include "SupplicantStaIfaceReq.h"

#include "aidl_manager.h"
#include "sta_iface.h"
#include "sta_network.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(function, status) \
    do { \
        LOG(INFO) << "Supplicant Someip Service: " << (function) \
                   << " return status: " << toString((status).code); \
    } while (0)

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool StaIfaceSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return SupplicantSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool StaIfaceMsgHandlerAddNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <AddNetwork> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::shared_ptr<ISupplicantStaNetwork> staNetwork; //Will not send back
    ndk::ScopedAStatus status = staiface_instance->addNetwork(&staNetwork);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    int32_t instanceId;
    if (status.isOk() && staNetwork != nullptr)
        std::dynamic_pointer_cast<StaNetwork>(staNetwork)->getId(&instanceId);
    LOG(INFO) << "Sending <AddNetwork> resp: (" << std::hex << instanceId << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeAddNetworkCfm(param, instanceId, outData);
}

bool StaIfaceMsgHandlerRemoveNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    if (!SupplicantStaIfaceParseRemoveNetworkReq(data, length - 4, id)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <RemoveNetwork>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <RemoveNetwork>: (" << id << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeNetwork(id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerListNetworks(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <ListNetworks> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<int32_t> networkList;
    ndk::ScopedAStatus status = staiface_instance->listNetworks(&networkList);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <ListNetworks> resp: (" << std::string(networkList.begin(), networkList.end()) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeListNetworksCfm(param, networkList, outData);
}

bool StaIfaceMsgHandlerReassociate(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <Reassociate> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->reassociate();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <Reconnect> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->reconnect();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <Disconnect> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->disconnect();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetPowerSave(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetPowerSaveReq(data, length - 4, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetPowerSave>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <SetPowerSave>: (" << enable << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setPowerSave(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerGetMacAddress(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <GetMacAddress> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    ndk::ScopedAStatus status = staiface_instance->getMacAddress(&macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    Vector2String(macAddress, temp);
    LOG(INFO) << "Sending <GetMacAddress> resp: (" << temp << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetMacAddressCfm(param, macAddress, outData);
}

bool StaIfaceMsgHandlerStartRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <StartRxFilter> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startRxFilter();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStopRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <StopRxFilter> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->stopRxFilter();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandleAddRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    RxFilterType type;
    if (!SupplicantStaIfaceParseAddRxFilterReq(data, length - 4, type)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <AddRxFilter>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <AddRxFilter>: (" << toString(type) << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->addRxFilter(type);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    RxFilterType type;
    if (!SupplicantStaIfaceParseRemoveRxFilterReq(data, length - 4, type)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <RemoveRxFilter>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <RemoveRxFilter>: (" << toString(type) << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeRxFilter(type);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetBtCoexistenceMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    BtCoexistenceMode mode;
    if (!SupplicantStaIfaceParseSetBtCoexistenceModeReq(data, length - 4, mode)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetBtCoexistenceMode>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <SetBtCoexistenceMode>: (" << toString(mode) << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setBtCoexistenceMode(mode);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetBtCoexistenceScanModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetBtCoexistenceScanModeEnabledReq(data, length - 4, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetBtCoexistenceScanModeEnabled>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <SetBtCoexistenceScanModeEnabled>: (" << enable << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setBtCoexistenceScanModeEnabled(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetSuspendModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetSuspendModeEnabledReq(data, length - 4, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetSuspendModeEnabled>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <SetSuspendModeEnabled>: (" << enable << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setSuspendModeEnabled(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetCountryCode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> code;
    if (!SupplicantStaIfaceParseSetCountryCodeReq(data, length - 4, code)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetCountryCode>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(code, temp);
    LOG(INFO) << "Processing StaIface Req <SetCountryCode>: (" << temp << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setCountryCode(code);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetExternalSim(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool useExternalSim;
    if (!SupplicantStaIfaceParseSetExternalSimReq(data, length - 4, useExternalSim)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <SetExternalSim>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <SetExternalSim>: (" << useExternalSim << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setExternalSim(useExternalSim);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerEnableAutoReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseEnableAutoReconnectReq(data, length - 4, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaIface Req <EnableAutoReconnect>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <EnableAutoReconnect>: (" << enable << ") for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->enableAutoReconnect(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerGetKeyMgmtCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <GetKeyMgmtCapabilities> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    KeyMgmtMask keyMask;
    ndk::ScopedAStatus status = staiface_instance->getKeyMgmtCapabilities(&keyMask);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetKeyMgmtCapabilities> resp: (" << toString(keyMask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetKeyMgmtCapabilitiesCfm(param, keyMask, outData);
}

bool StaIfaceMsgHandlerGetConnectionCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <GetConnectionCapabilities> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ConnectionCapabilities connectionCapabilities;
    ndk::ScopedAStatus status = staiface_instance->getConnectionCapabilities(&connectionCapabilities);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

//    LOG(INFO) << "Sending <GetConnectionCapabilities> resp: (" << toString(capabilityMask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetConnectionCapabilitiesCfm(param, connectionCapabilities, outData);
}

bool StaIfaceMsgHandlerGetWpaDriverCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <GetWpaDriverCapabilities> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    WpaDriverCapabilitiesMask capabilityMask;
    ndk::ScopedAStatus status = staiface_instance->getWpaDriverCapabilities(&capabilityMask);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetWpaDriverCapabilities> resp: (" << toString(capabilityMask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetWpaDriverCapabilitiesCfm(param, capabilityMask, outData);
}

bool StaIfaceMsgHandlerGetSignalPollResults(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaIface Req <GetSignalPollResults> for iface --> " << ifName;

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        LOG(ERROR) << "[Fail] Interface not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<SignalPollResult> signalPollResult;
    ndk::ScopedAStatus status = staiface_instance->getSignalPollResults(&signalPollResult);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

//    LOG(INFO) << "Sending <GetSignalPollResults> resp: (" << toString(capabilityMask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetSignalPollResultsCfm(param, signalPollResult, outData);
}

