/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/common_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_sta_iface_msg.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "SupplicantStaIfaceReq.h"

#include "aidl_manager.h"
#include "sta_iface.h"
#include "sta_network.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(function, status) \
    do { \
        ALOGI("Supplicant Someip Service: <%s>, return status: (%s)", __func__, toString((status).code).c_str()); \
    } while (0)

static inline void v2s(const vector<uint8_t>& v, string& s)
{
    std::stringstream buffer;
    buffer << " ";
    for (const auto& value : v) {
        buffer << std::setfill('0') << std::setw(2) << std::hex << value+0 << " ";
    }
    s = buffer.str();
}

static inline void ScopedAStatus2HalStatusParam(const ndk::ScopedAStatus& status, HalStatusParam& param)
{
    param.status = (int32_t)status.code;
    param.info = status.description;
}

static inline bool StaIfaceSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return SupplicantSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool StaIfaceMsgHandlerAddDppPeerUri(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string uri;
    if (!SupplicantStaIfaceParseAddDppPeerUriReq(data, length - 4, uri)) {
        ALOGE("[Fail] Parsing StaIface Req <AddDppPeerUri>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <AddDppPeerUri>: (%s) for iface --> %s", uri.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    ndk::ScopedAStatus status = staiface_instance->addDppPeerUri(uri, &id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <AddDppPeerUri> resp: (%d)", id);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeAddDppPeerUriCfm(param, id, outData);
}

bool StaIfaceMsgHandlerAddExtRadioWork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    AddExtRadioWorkReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseAddExtRadioWorkReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <AddExtRadioWork>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <AddExtRadioWork>: (%s), (%d), (%d) for iface --> %s", buff.name.c_str(), buff.freqInMhz, buff.timeoutInSec, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    ndk::ScopedAStatus status = staiface_instance->addExtRadioWork(buff.name, buff.freqInMhz, buff.timeoutInSec, &id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <AddExtRadioWork> resp: (%d)", id);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeAddExtRadioWorkCfm(param, id, outData);
}

bool StaIfaceMsgHandlerAddNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <AddNetwork> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::shared_ptr<ISupplicantStaNetwork> staNetwork; //Will not send back
    ndk::ScopedAStatus status = staiface_instance->addNetwork(&staNetwork);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    int32_t instanceId;
    if (status.isOk() && staNetwork != nullptr)
        std::dynamic_pointer_cast<StaNetwork>(staNetwork)->getId(&instanceId);
    ALOGI("Sending <AddNetwork> resp: (%d)", instanceId);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeAddNetworkCfm(param, instanceId, outData);
}

bool StaIfaceMsgHandlerAddRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    RxFilterType type;
    if (!SupplicantStaIfaceParseAddRxFilterReq(data, length - 4, type)) {
        ALOGE("[Fail] Parsing StaIface Req <AddRxFilter>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <AddRxFilter>: (%s) for iface --> %s", toString(type).c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->addRxFilter(type);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerCancelWps(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <CancelWps> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->cancelWps();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <Disconnect> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->disconnect();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerEnableAutoReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseEnableAutoReconnectReq(data, length - 4, enable)) {
        ALOGE("[Fail] Parsing StaIface Req <EnableAutoReconnect>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <EnableAutoReconnect>: (%d) for iface --> %s", enable, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->enableAutoReconnect(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerFilsHlpAddRequest(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    FilsHlpAddRequestReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseFilsHlpAddRequestReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <FilsHlpAddRequest>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.dst_mac, temp);
    ALOGI("Processing StaIface Req <FilsHlpAddRequest>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->filsHlpAddRequest(buff.dst_mac, buff.pkt);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerFilsHlpFlushRequest(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <FilsHlpFlushRequest> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->filsHlpFlushRequest();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerGenerateDppBootstrapInfoForResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    GenerateDppBootstrapInfoForResponderReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseGenerateDppBootstrapInfoForResponderReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <GenerateDppBootstrapInfoForResponder>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.macAddress, temp);
    ALOGI("Processing StaIface Req <GenerateDppBootstrapInfoForResponder>: (%s), (%s), (%s) for iface --> %s", temp.c_str(), buff.deviceInfo.c_str(), toString(buff.curve).c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    DppResponderBootstrapInfo dppResponderBootstrapInfo;
    ndk::ScopedAStatus status = staiface_instance->generateDppBootstrapInfoForResponder(buff.macAddress, buff.deviceInfo, buff.curve, &dppResponderBootstrapInfo);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GenerateDppBootstrapInfoForResponder> resp: (%d)", dppResponderBootstrapInfo.bootstrapId);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGenerateDppBootstrapInfoForResponderCfm(param, dppResponderBootstrapInfo, outData);
}

bool StaIfaceMsgHandlerGenerateSelfDppConfiguration(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    GenerateSelfDppConfigurationReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseGenerateSelfDppConfigurationReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <GenerateSelfDppConfiguration>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.privEcKey, temp);
    ALOGI("Processing StaIface Req <GenerateSelfDppConfiguration>: (%s), (%s) for iface --> %s", buff.ssid.c_str(), temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->generateSelfDppConfiguration(buff.ssid, buff.privEcKey);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerGetConnectionCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetConnectionCapabilities> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ConnectionCapabilities connectionCapabilities;
    ndk::ScopedAStatus status = staiface_instance->getConnectionCapabilities(&connectionCapabilities);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetConnectionCapabilities> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetConnectionCapabilitiesCfm(param, connectionCapabilities, outData);
}

bool StaIfaceMsgHandlerGetConnectionMloLinksInfo(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetConnectionMloLinksInfo> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    MloLinksInfo mloLinksInfo;
    ndk::ScopedAStatus status = staiface_instance->getConnectionMloLinksInfo(&mloLinksInfo);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetConnectionMloLinksInfo> resp: (%d)", mloLinksInfo.apMloLinkId);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetConnectionMloLinksInfoCfm(param, mloLinksInfo, outData);
}

bool StaIfaceMsgHandlerGetKeyMgmtCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetKeyMgmtCapabilities> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    KeyMgmtMask keyMask;
    ndk::ScopedAStatus status = staiface_instance->getKeyMgmtCapabilities(&keyMask);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetKeyMgmtCapabilities> resp: (%s)", toString(keyMask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetKeyMgmtCapabilitiesCfm(param, keyMask, outData);
}

bool StaIfaceMsgHandlerGetMacAddress(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetMacAddress> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    ndk::ScopedAStatus status = staiface_instance->getMacAddress(&macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(macAddress, temp);
    ALOGI("Sending <GetMacAddress> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetMacAddressCfm(param, macAddress, outData);
}

bool StaIfaceMsgHandlerGetName(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetName> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string name;
    ndk::ScopedAStatus status = staiface_instance->getName(&name);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetName> resp: (%s)", name.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetNameCfm(param, name, outData);
}

bool StaIfaceMsgHandlerGetType(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetType> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    IfaceType ifaceType;
    ndk::ScopedAStatus status = staiface_instance->getType(&ifaceType);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetType> resp: (%s)", toString(ifaceType).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetTypeCfm(param, ifaceType, outData);
}

bool StaIfaceMsgHandlerGetWpaDriverCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetWpaDriverCapabilities> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    WpaDriverCapabilitiesMask capabilityMask;
    ndk::ScopedAStatus status = staiface_instance->getWpaDriverCapabilities(&capabilityMask);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetWpaDriverCapabilities> resp: (%s)", toString(capabilityMask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetWpaDriverCapabilitiesCfm(param, capabilityMask, outData);
}

bool StaIfaceMsgHandlerInitiateAnqpQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    InitiateAnqpQueryReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseInitiateAnqpQueryReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateAnqpQuery>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateAnqpQuery>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateAnqpQuery(buff.macAddress, buff.infoElements, buff.subTypes);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerInitiateHs20IconQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    InitiateHs20IconQueryReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseInitiateHs20IconQueryReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateHs20IconQuery>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateHs20IconQuery>: (%s), (%s) for iface --> %s", temp.c_str(), buff.fileName.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateHs20IconQuery(buff.macAddress, buff.fileName);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerInitiateTdlsDiscover(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    if (!SupplicantStaIfaceParseInitiateTdlsDiscoverReq(data, length - 4, macAddress)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateTdlsDiscover>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateTdlsDiscover>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateTdlsDiscover(macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerInitiateTdlsSetup(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    if (!SupplicantStaIfaceParseInitiateTdlsSetupReq(data, length - 4, macAddress)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateTdlsSetup>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateTdlsSetup>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateTdlsSetup(macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerInitiateTdlsTeardown(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    if (!SupplicantStaIfaceParseInitiateTdlsTeardownReq(data, length - 4, macAddress)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateTdlsTeardown>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateTdlsTeardown>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateTdlsTeardown(macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerInitiateVenueUrlAnqpQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> macAddress;
    if (!SupplicantStaIfaceParseInitiateVenueUrlAnqpQueryReq(data, length - 4, macAddress)) {
        ALOGE("[Fail] Parsing StaIface Req <InitiateVenueUrlAnqpQuery>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(macAddress, temp);
    ALOGI("Processing StaIface Req <InitiateVenueUrlAnqpQuery>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->initiateVenueUrlAnqpQuery(macAddress);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerListNetworks(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <ListNetworks> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<int32_t> networkList;
    ndk::ScopedAStatus status = staiface_instance->listNetworks(&networkList);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <ListNetworks> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeListNetworksCfm(param, networkList, outData);
}

bool StaIfaceMsgHandlerReassociate(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <Reassociate> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
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
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <Reconnect> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->reconnect();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetQosPolicyFeatureEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetQosPolicyFeatureEnabledReq(data, length - 4, enable)) {
        ALOGE("[Fail] Parsing StaIface Req <SetQosPolicyFeatureEnabled>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetQosPolicyFeatureEnabled>: (%d) for iface --> %s", enable, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setQosPolicyFeatureEnabled(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSendQosPolicyResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    SendQosPolicyResponseReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseSendQosPolicyResponseReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <SendQosPolicyResponse>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SendQosPolicyResponse>: (%d), (%d) for iface --> %s", buff.qosPolicyRequestId, buff.morePolicies, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->sendQosPolicyResponse(buff.qosPolicyRequestId, buff.morePolicies, buff.qosPolicyStatusList);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveAllQosPolicies(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <RemoveAllQosPolicies> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeAllQosPolicies();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveDppUri(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    if (!SupplicantStaIfaceParseRemoveDppUriReq(data, length - 4, id)) {
        ALOGE("[Fail] Parsing StaIface Req <RemoveDppUri>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <RemoveDppUri>: (%d) for iface --> %s", id, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeDppUri(id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveExtRadioWork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    if (!SupplicantStaIfaceParseRemoveExtRadioWorkReq(data, length - 4, id)) {
        ALOGE("[Fail] Parsing StaIface Req <RemoveExtRadioWork>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <RemoveExtRadioWork>: (%d) for iface --> %s", id, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeExtRadioWork(id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t id;
    if (!SupplicantStaIfaceParseRemoveNetworkReq(data, length - 4, id)) {
        ALOGE("[Fail] Parsing StaIface Req <RemoveNetwork>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <RemoveNetwork>: (%d) for iface --> %s", id, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->removeNetwork(id);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerRemoveRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    RxFilterType type;
    if (!SupplicantStaIfaceParseRemoveRxFilterReq(data, length - 4, type)) {
        ALOGE("[Fail] Parsing StaIface Req <RemoveRxFilter>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <RemoveRxFilter>: (%s) for iface --> %s", toString(type).c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
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
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    BtCoexistenceMode mode;
    if (!SupplicantStaIfaceParseSetBtCoexistenceModeReq(data, length - 4, mode)) {
        ALOGE("[Fail] Parsing StaIface Req <SetBtCoexistenceMode>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetBtCoexistenceMode>: (%s) for iface --> %s", toString(mode).c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
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
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetBtCoexistenceScanModeEnabledReq(data, length - 4, enable)) {
        ALOGE("[Fail] Parsing StaIface Req <SetBtCoexistenceScanModeEnabled>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetBtCoexistenceScanModeEnabled>: (%d) for iface --> %s", enable, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setBtCoexistenceScanModeEnabled(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetCountryCode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> code;
    if (!SupplicantStaIfaceParseSetCountryCodeReq(data, length - 4, code)) {
        ALOGE("[Fail] Parsing StaIface Req <SetCountryCode>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(code, temp);
    ALOGI("Processing StaIface Req <SetCountryCode>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
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
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool useExternalSim;
    if (!SupplicantStaIfaceParseSetExternalSimReq(data, length - 4, useExternalSim)) {
        ALOGE("[Fail] Parsing StaIface Req <SetExternalSim>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetExternalSim>: (%d) for iface --> %s", useExternalSim, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setExternalSim(useExternalSim);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetMboCellularDataStatus(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool available;
    if (!SupplicantStaIfaceParseSetMboCellularDataStatusReq(data, length - 4, available)) {
        ALOGE("[Fail] Parsing StaIface Req <SetMboCellularDataStatus>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetMboCellularDataStatus>: (%d) for iface --> %s", available, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setMboCellularDataStatus(available);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetPowerSave(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetPowerSaveReq(data, length - 4, enable)) {
        ALOGE("[Fail] Parsing StaIface Req <SetPowerSave>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetPowerSave>: (%d) for iface --> %s", enable, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setPowerSave(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetSuspendModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaIfaceParseSetSuspendModeEnabledReq(data, length - 4, enable)) {
        ALOGE("[Fail] Parsing StaIface Req <SetSuspendModeEnabled>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetSuspendModeEnabled>: (%d) for iface --> %s", enable, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setSuspendModeEnabled(enable);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsConfigMethods(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    WpsConfigMethods configMethods;
    if (!SupplicantStaIfaceParseSetWpsConfigMethodsReq(data, length - 4, configMethods)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsConfigMethods>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsConfigMethods>: (%s) for iface --> %s", toString(configMethods).c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsConfigMethods(configMethods);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsDeviceName(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string name;
    if (!SupplicantStaIfaceParseSetWpsDeviceNameReq(data, length - 4, name)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsDeviceName>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsDeviceName>: (%s) for iface --> %s", name.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsDeviceName(name);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsDeviceType(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> type;
    if (!SupplicantStaIfaceParseSetWpsDeviceTypeReq(data, length - 4, type)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsDeviceType>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(type, temp);
    ALOGI("Processing StaIface Req <SetWpsDeviceType>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsDeviceType(type);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsManufacturer(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string manufacturer;
    if (!SupplicantStaIfaceParseSetWpsManufacturerReq(data, length - 4, manufacturer)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsManufacturer>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsManufacturer>: (%s) for iface --> %s", manufacturer.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsManufacturer(manufacturer);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsModelName(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string modelName;
    if (!SupplicantStaIfaceParseSetWpsModelNameReq(data, length - 4, modelName)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsModelName>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsModelName>: (%s) for iface --> %s", modelName.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsModelName(modelName);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsModelNumber(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string modelNumber;
    if (!SupplicantStaIfaceParseSetWpsModelNumberReq(data, length - 4, modelNumber)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsModelNumber>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsModelNumber>: (%s) for iface --> %s", modelNumber.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsModelNumber(modelNumber);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerSetWpsSerialNumber(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string serialNumber;
    if (!SupplicantStaIfaceParseSetWpsSerialNumberReq(data, length - 4, serialNumber)) {
        ALOGE("[Fail] Parsing StaIface Req <SetWpsSerialNumber>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <SetWpsSerialNumber>: (%s) for iface --> %s", serialNumber.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->setWpsSerialNumber(serialNumber);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartDppConfiguratorInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    StartDppConfiguratorInitiatorReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseStartDppConfiguratorInitiatorReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <StartDppConfiguratorInitiator>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StartDppConfiguratorInitiator>: (%s) for iface --> %s", buff.ssid.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> key;
    ndk::ScopedAStatus status = staiface_instance->startDppConfiguratorInitiator(buff.peerBootstrapId, buff.ownBootstrapId, buff.ssid, buff.password, buff.psk, buff.netRole, buff.securityAkm, buff.privEcKey, &key);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(key, temp);
    ALOGI("Sending <StartDppConfiguratorInitiator> resp: (%d)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeStartDppConfiguratorInitiatorCfm(param, key, outData);
}

bool StaIfaceMsgHandlerStartDppEnrolleeInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    StartDppEnrolleeInitiatorReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseStartDppEnrolleeInitiatorReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <StartDppEnrolleeInitiator>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StartDppEnrolleeInitiator>: (%d), (%d) for iface --> %s", buff.peerBootstrapId, buff.ownBootstrapId, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startDppEnrolleeInitiator(buff.peerBootstrapId, buff.ownBootstrapId);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartDppEnrolleeResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t listenChannel;
    if (!SupplicantStaIfaceParseStartDppEnrolleeResponderReq(data, length - 4, listenChannel)) {
        ALOGE("[Fail] Parsing StaIface Req <StartDppEnrolleeResponder>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StartDppEnrolleeResponder>: (%d) for iface --> %s", listenChannel, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startDppEnrolleeResponder(listenChannel);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StartRxFilter> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startRxFilter();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartWpsPbc(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    if (!SupplicantStaIfaceParseStartWpsPbcReq(data, length - 4, bssid)) {
        ALOGE("[Fail] Parsing StaIface Req <StartWpsPbc>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(bssid, temp);
    ALOGI("Processing StaIface Req <StartWpsPbc>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startWpsPbc(bssid);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartWpsPinDisplay(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    if (!SupplicantStaIfaceParseStartWpsPinDisplayReq(data, length - 4, bssid)) {
        ALOGE("[Fail] Parsing StaIface Req <StartWpsPinDisplay>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(bssid, temp);
    ALOGI("Processing StaIface Req <StartWpsPinDisplay>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string wpsPin;
    ndk::ScopedAStatus status = staiface_instance->startWpsPinDisplay(bssid, &wpsPin);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <StartWpsPinDisplay> resp: (%s)", wpsPin.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeStartWpsPinDisplayCfm(param, wpsPin, outData);
}

bool StaIfaceMsgHandlerStartWpsPinKeypad(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string pin;
    if (!SupplicantStaIfaceParseStartWpsPinKeypadReq(data, length - 4, pin)) {
        ALOGE("[Fail] Parsing StaIface Req <StartWpsPinKeypad>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StartWpsPinKeypad>: (%s) for iface --> %s", pin.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startWpsPinKeypad(pin);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStartWpsRegistrar(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    StartWpsRegistrarReqStaIfaceParam buff;
    if (!SupplicantStaIfaceParseStartWpsRegistrarReq(data, length - 4, buff)) {
        ALOGE("[Fail] Parsing StaIface Req <StartWpsRegistrar>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.bssid, temp);
    ALOGI("Processing StaIface Req <StartWpsRegistrar>: (%s), (%s) for iface --> %s", temp.c_str(), buff.pin.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->startWpsRegistrar(buff.bssid, buff.pin);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStopDppInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StopDppInitiator> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->stopDppInitiator();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStopDppResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    int32_t ownBootstrapId;
    if (!SupplicantStaIfaceParseStopDppResponderReq(data, length - 4, ownBootstrapId)) {
        ALOGE("[Fail] Parsing StaIface Req <StopDppResponder>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StopDppResponder>: (%d) for iface --> %s", ownBootstrapId, ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->stopDppResponder(ownBootstrapId);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerStopRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <StopRxFilter> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = staiface_instance->stopRxFilter();
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    return StaIfaceSerializeStatus(status, outData);
}

bool StaIfaceMsgHandlerGetSignalPollResults(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <GetSignalPollResults> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<SignalPollResult> signalPollResult;
    ndk::ScopedAStatus status = staiface_instance->getSignalPollResults(&signalPollResult);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetSignalPollResults> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeGetSignalPollResultsCfm(param, signalPollResult, outData);
}

bool StaIfaceMsgHandlerAddQosPolicyRequestForScs(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<QosPolicyScsData> qosPolicyData;
    if (!SupplicantStaIfaceParseAddQosPolicyRequestForScsReq(data, length - 4, qosPolicyData)) {
        ALOGE("[Fail] Parsing StaIface Req <AddQosPolicyRequestForScs>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    ALOGI("Processing StaIface Req <AddQosPolicyRequestForScs> for iface --> %s", ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<QosPolicyScsRequestStatus> reports;
    ndk::ScopedAStatus status = staiface_instance->addQosPolicyRequestForScs(qosPolicyData, &reports);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <AddQosPolicyRequestForScs> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeAddQosPolicyRequestForScsCfm(param, reports, outData);
}

bool StaIfaceMsgHandlerRemoveQosPolicyForScs(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 4) {
        ALOGE("Invalid data payload length: (%zu)", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<uint8_t> scsPolicyIds;
    if (!SupplicantStaIfaceParseRemoveQosPolicyForScsReq(data, length - 4, scsPolicyIds)) {
        ALOGE("[Fail] Parsing StaIface Req <RemoveQosPolicyForScs>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(scsPolicyIds, temp);
    ALOGI("Processing StaIface Req <RemoveQosPolicyForScs>: (%s) for iface --> %s", temp.c_str(), ifName.c_str());

    std::shared_ptr<ISupplicantStaIface> staiface_instance;
    if(aidl_manager->getStaIfaceAidlObjectByIfname(ifName, &staiface_instance)) {
        ALOGE("[Fail] Interface not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_IFACE_INVALID);
        return StaIfaceSerializeStatus(status, outData);
    }

    std::vector<QosPolicyScsRequestStatus> reports;
    ndk::ScopedAStatus status = staiface_instance->removeQosPolicyForScs(scsPolicyIds, &reports);
    SUPPLICANT_STAIFACE_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <RemoveQosPolicyForScs> resp");

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaIfaceSerializeRemoveQosPolicyForScsCfm(param, reports, outData);
}
