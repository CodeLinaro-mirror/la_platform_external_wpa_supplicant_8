/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_sta_network_msg.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "SupplicantStaNetworkReq.h"

#include "aidl_manager.h"
#include "sta_network.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(function, status) \
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

static inline bool StaNetworkSerializeStatus(const ndk::ScopedAStatus& status, std::vector<uint8_t>& outData)
{
    return SupplicantSerializeHalStatus((int32_t)status.code, status.description, outData);
}

bool StaNetworkMsgHandlerGetId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetId> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t id;
    ndk::ScopedAStatus status = stanetwork_instance->getId(&id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetId> resp: (%d)", id);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetIdCfm(param, id, outData);
}

bool StaNetworkMsgHandlerSetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> ssid;
    if (!SupplicantStaNetworkParseSetSsidReq(data, length - 8, ssid)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetSsid>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(ssid, temp);
    ALOGI("Processing StaNetwork Req <SetSsid>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setSsid(ssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    if (!SupplicantStaNetworkParseSetBssidReq(data, length - 8, bssid)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetBssid>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(bssid, temp);
    ALOGI("Processing StaNetwork Req <SetBssid>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setBssid(bssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetScanSsidReq(data, length - 8, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetScanSsid>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetScanSsid>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setScanSsid(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    KeyMgmtMask keyMgmtMask;
    if (!SupplicantStaNetworkParseSetKeyMgmtReq(data, length - 8, keyMgmtMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetKeyMgmt>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetKeyMgmt>: (%s) for network -> %d of iface --> %s", toString(keyMgmtMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setKeyMgmt(keyMgmtMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ProtoMask protoMask;
    if (!SupplicantStaNetworkParseSetProtoReq(data, length - 8, protoMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetProto>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetProto>: (%s) for network -> %d of iface --> %s", toString(protoMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setProto(protoMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    AuthAlgMask authAlgMask;
    if (!SupplicantStaNetworkParseSetAuthAlgReq(data, length - 8, authAlgMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetAuthAlg>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetAuthAlg>: (%s) for network -> %d of iface --> %s", toString(authAlgMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setAuthAlg(authAlgMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupCipherMask groupCipherMask;
    if (!SupplicantStaNetworkParseSetGroupCipherReq(data, length - 8, groupCipherMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetGroupCipher>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetGroupCipher>: (%s) for network -> %d of iface --> %s", toString(groupCipherMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setGroupCipher(groupCipherMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    PairwiseCipherMask pairwiseCipherMask;
    if (!SupplicantStaNetworkParseSetPairwiseCipherReq(data, length - 8, pairwiseCipherMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetPairwiseCipher>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetPairwiseCipher>: (%s) for network -> %d of iface --> %s", toString(pairwiseCipherMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setPairwiseCipher(pairwiseCipherMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string psk;
    if (!SupplicantStaNetworkParseSetPskPassphraseReq(data, length - 8, psk)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetPskPassphrase>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetPskPassphrase>: (%s) for network -> %d of iface --> %s", psk.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setPskPassphrase(psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> psk;
    if (!SupplicantStaNetworkParseSetPskReq(data, length - 8, psk)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetPsk>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(psk, temp);
    ALOGI("Processing StaNetwork Req <SetPsk>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setPsk(psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SetWepKeyReqStaNetworkParam buff;
    if (!SupplicantStaNetworkParseSetWepKeyReq(data, length - 8, buff)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetWepKey>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(buff.wepKey, temp);
    ALOGI("Processing StaNetwork Req <SetWepKey>: (%d), (%s) for network -> %d of iface --> %s", buff.keyIdx, temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setWepKey(buff.keyIdx, buff.wepKey);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetWepTxKeyIdx(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseSetWepTxKeyIdxReq(data, length - 8, keyIdx)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetWepTxKeyIdx>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetWepTxKeyIdx>: (%d) for network -> %d of iface --> %s", keyIdx, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setWepTxKeyIdx(keyIdx);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetRequirePmfReq(data, length - 8, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetRequirePmf>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetRequirePmf>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setRequirePmf(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string idStr;
    if (!SupplicantStaNetworkParseSetIdStrReq(data, length - 8, idStr)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetIdStr>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetIdStr>: (%s) for network -> %d of iface --> %s", idStr.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setIdStr(idStr);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerGetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetSsid> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> ssid;
    ndk::ScopedAStatus status = stanetwork_instance->getSsid(&ssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(ssid, temp);
    ALOGI("Sending <GetSsid> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetSsidCfm(param, ssid, outData);
}

bool StaNetworkMsgHandlerGetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetBssid> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    ndk::ScopedAStatus status = stanetwork_instance->getBssid(&bssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(bssid, temp);
    ALOGI("Sending <GetBssid> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetBssidCfm(param, bssid, outData);
}

bool StaNetworkMsgHandlerGetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetScanSsid> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getScanSsid(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetScanSsid> resp: (%d)", res);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetScanSsidCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetKeyMgmt> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    KeyMgmtMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getKeyMgmt(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetKeyMgmt> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetKeyMgmtCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetProto> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ProtoMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getProto(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetProto> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetProtoCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetAuthAlg> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    AuthAlgMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getAuthAlg(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetAuthAlg> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetAuthAlgCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetGroupCipher> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getGroupCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetGroupCipher> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetGroupCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetPairwiseCipher> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    PairwiseCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getPairwiseCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetPairwiseCipher> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPairwiseCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetPskPassphrase> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string psk;
    ndk::ScopedAStatus status = stanetwork_instance->getPskPassphrase(&psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetPskPassphrase> resp: (%s)", psk.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPskPassphraseCfm(param, psk, outData);
}

bool StaNetworkMsgHandlerGetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetPsk> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> psk;
    ndk::ScopedAStatus status = stanetwork_instance->getPsk(&psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(psk, temp);
    ALOGI("Sending <GetPsk> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPskCfm(param, psk, outData);
}

bool StaNetworkMsgHandlerGetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetSaePassword> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePw;
    ndk::ScopedAStatus status = stanetwork_instance->getSaePassword(&saePw);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetSaePassword> resp: (%s)", saePw.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetSaePasswordCfm(param, saePw, outData);
}

bool StaNetworkMsgHandlerGetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseGetWepKeyReq(data, length - 8, keyIdx)) {
        ALOGE("[Fail] Parsing StaNetwork Req <GetWepKey>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetWepKey>: (%d) for network -> %d of iface --> %s", keyIdx, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> key;
    ndk::ScopedAStatus status = stanetwork_instance->getWepKey(keyIdx, &key);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(key, temp);
    ALOGI("Sending <GetWepKey> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWepKeyCfm(param, key, outData);
}

bool StaNetworkMsgHandlerGetWepTxKeyIdx(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetWepTxKeyIdx> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t index;
    ndk::ScopedAStatus status = stanetwork_instance->getWepTxKeyIdx(&index);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetWepTxKeyIdx> resp: (%d)", index);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWepTxKeyIdxCfm(param, index, outData);
}

bool StaNetworkMsgHandlerGetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetRequirePmf> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getRequirePmf(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetRequirePmf> resp: (%d)", res);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetRequirePmfCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetIdStr> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string id;
    ndk::ScopedAStatus status = stanetwork_instance->getIdStr(&id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetIdStr> resp: (%s)", id.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetIdStrCfm(param, id, outData);
}

bool StaNetworkMsgHandlerEnable(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool noConnect;
    if (!SupplicantStaNetworkParseEnableReq(data, length - 8, noConnect)) {
        ALOGE("[Fail] Parsing StaNetwork Req <Enable>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <Enable>: (%d) for network -> %d of iface --> %s", noConnect, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->enable(noConnect);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerDisable(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <Disable> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->disable();
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSelect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <Select> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->select();
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{    
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupMgmtCipherMask groupMgmtCipherMask;
    if (!SupplicantStaNetworkParseSetGroupMgmtCipherReq(data, length - 8, groupMgmtCipherMask)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetGroupMgmtCipher>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetGroupMgmtCipher>: (%s) for network -> %d of iface --> %s", toString(groupMgmtCipherMask).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setGroupMgmtCipher(groupMgmtCipherMask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}


bool StaNetworkMsgHandlerGetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetGroupMgmtCipher> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupMgmtCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getGroupMgmtCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetGroupMgmtCipher> resp: (%s)", toString(mask).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetGroupMgmtCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerSetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePassword;
    if (!SupplicantStaNetworkParseSetSaePasswordReq(data, length - 8, saePassword)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetSaePassword>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetSaePassword>: (%s) for network -> %d of iface --> %s", saePassword.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setSaePassword(saePassword);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetPmkCache(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> serializedEntry;
    if (!SupplicantStaNetworkParseSetPmkCacheReq(data, length - 8, serializedEntry)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetPmkCache>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(serializedEntry, temp);
    ALOGI("Processing StaNetwork Req <SetPmkCache>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setPmkCache(serializedEntry);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}


bool StaNetworkMsgHandlerSetSaeH2eMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{    
    if (!data || length < 8) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SaeH2eMode mode;
    if (!SupplicantStaNetworkParseSetSaeH2eModeReq(data, length - 8, mode)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetSaeH2eMode>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetSaeH2eMode>: (%s) for network -> %d of iface --> %s", toString(mode).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setSaeH2eMode(mode);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}
