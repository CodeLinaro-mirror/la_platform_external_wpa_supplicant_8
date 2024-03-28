/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <android-base/logging.h>

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/message/wpa_supplicant/supplicant_sta_network_msg.h>

#include "SupplicantStaNetworkReq.h"

#include "aidl_manager.h"
#include "sta_network.h"

using namespace aidl::android::hardware::wifi::supplicant;

#define SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(function, status) \
    do { \
        LOG(INFO) << "Supplicant Someip Service: " << (function) \
                   << " return status: " << toString((status).code); \
    } while (0)

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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetId> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t id;
    ndk::ScopedAStatus status = stanetwork_instance->getId(&id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetId> resp: (" << id << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetIdCfm(param, id, outData);
}

bool StaNetworkMsgHandlerSetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> ssid;
    if (!SupplicantStaNetworkParseSetSsidReq(data, length - 8, ssid)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetSsid>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(ssid, temp);
    LOG(INFO) << "Processing StaNetwork Req <SetSsid>: (" << temp << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    if (!SupplicantStaNetworkParseSetBssidReq(data, length - 8, bssid)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetBssid>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(bssid, temp);
    LOG(INFO) << "Processing StaNetwork Req <SetBssid>: (" << temp << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetScanSsidReq(data, length - 8, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetScanSsid>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetScanSsid>: (" << enable << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    KeyMgmtMask keyMgmtMask;
    if (!SupplicantStaNetworkParseSetKeyMgmtReq(data, length - 8, keyMgmtMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetKeyMgmt>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetKeyMgmt>: (" << toString(keyMgmtMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ProtoMask protoMask;
    if (!SupplicantStaNetworkParseSetProtoReq(data, length - 8, protoMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetProto>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetProto>: (" << toString(protoMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    AuthAlgMask authAlgMask;
    if (!SupplicantStaNetworkParseSetAuthAlgReq(data, length - 8, authAlgMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetAuthAlg>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetAuthAlg>: (" << toString(authAlgMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupCipherMask groupCipherMask;
    if (!SupplicantStaNetworkParseSetGroupCipherReq(data, length - 8, groupCipherMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetGroupCipher>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetGroupCipher>: (" << toString(groupCipherMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    PairwiseCipherMask pairwiseCipherMask;
    if (!SupplicantStaNetworkParseSetPairwiseCipherReq(data, length - 8, pairwiseCipherMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetPairwiseCipher>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetPairwiseCipher>: (" << toString(pairwiseCipherMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string psk;
    if (!SupplicantStaNetworkParseSetPskPassphraseReq(data, length - 8, psk)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetPskPassphrase>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetPskPassphrase>: (" << psk << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> psk;
    if (!SupplicantStaNetworkParseSetPskReq(data, length - 8, psk)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetPsk>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(psk, temp);
    LOG(INFO) << "Processing StaNetwork Req <SetPsk>: (" << temp << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SetWepKeyReqStaNetworkParam buff;
    if (!SupplicantStaNetworkParseSetWepKeyReq(data, length - 8, buff)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetWepKey>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(buff.wepKey, temp);
    LOG(INFO) << "Processing StaNetwork Req <SetWepKey>: (" << buff.keyIdx << "), (" << temp << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseSetWepTxKeyIdxReq(data, length - 8, keyIdx)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetWepTxKeyIdx>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetWepTxKeyIdx>: (" << keyIdx << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetRequirePmfReq(data, length - 8, enable)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetRequirePmf>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetRequirePmf>: (" << enable << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string idStr;
    if (!SupplicantStaNetworkParseSetIdStrReq(data, length - 8, idStr)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetIdStr>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetIdStr>: (" << idStr << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetSsid> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> ssid;
    ndk::ScopedAStatus status = stanetwork_instance->getSsid(&ssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    Vector2String(ssid, temp);
    LOG(INFO) << "Sending <GetSsid> resp: (" << temp << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetSsidCfm(param, ssid, outData);
}

bool StaNetworkMsgHandlerGetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetBssid> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    ndk::ScopedAStatus status = stanetwork_instance->getBssid(&bssid);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    Vector2String(bssid, temp);
    LOG(INFO) << "Sending <GetBssid> resp: (" << temp << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetBssidCfm(param, bssid, outData);
}

bool StaNetworkMsgHandlerGetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetScanSsid> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getScanSsid(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetScanSsid> resp: (" << res << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetScanSsidCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetKeyMgmt> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    KeyMgmtMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getKeyMgmt(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetKeyMgmt> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetKeyMgmtCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetProto> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ProtoMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getProto(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetProto> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetProtoCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetAuthAlg> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    AuthAlgMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getAuthAlg(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetAuthAlg> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetAuthAlgCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetGroupCipher> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getGroupCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetGroupCipher> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetGroupCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetPairwiseCipher> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    PairwiseCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getPairwiseCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetPairwiseCipher> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPairwiseCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerGetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetPskPassphrase> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string psk;
    ndk::ScopedAStatus status = stanetwork_instance->getPskPassphrase(&psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetPskPassphrase> resp: (" << psk << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPskPassphraseCfm(param, psk, outData);
}

bool StaNetworkMsgHandlerGetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetPsk> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> psk;
    ndk::ScopedAStatus status = stanetwork_instance->getPsk(&psk);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    Vector2String(psk, temp);
    LOG(INFO) << "Sending <GetPsk> resp: (" << temp << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetPskCfm(param, psk, outData);
}

bool StaNetworkMsgHandlerGetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetSaePassword> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePw;
    ndk::ScopedAStatus status = stanetwork_instance->getSaePassword(&saePw);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetSaePassword> resp: (" << saePw << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetSaePasswordCfm(param, saePw, outData);
}

bool StaNetworkMsgHandlerGetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseGetWepKeyReq(data, length - 8, keyIdx)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <GetWepKey>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetWepKey>: (" << keyIdx << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> key;
    ndk::ScopedAStatus status = stanetwork_instance->getWepKey(keyIdx, &key);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    Vector2String(key, temp);
    LOG(INFO) << "Sending <GetWepKey> resp: (" << temp << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWepKeyCfm(param, key, outData);
}

bool StaNetworkMsgHandlerGetWepTxKeyIdx(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetWepTxKeyIdx> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t index;
    ndk::ScopedAStatus status = stanetwork_instance->getWepTxKeyIdx(&index);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetWepTxKeyIdx> resp: (" << index << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWepTxKeyIdxCfm(param, index, outData);
}

bool StaNetworkMsgHandlerGetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetRequirePmf> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getRequirePmf(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetRequirePmf> resp: (" << res << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetRequirePmfCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetIdStr> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string id;
    ndk::ScopedAStatus status = stanetwork_instance->getIdStr(&id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetIdStr> resp: (" << id << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetIdStrCfm(param, id, outData);
}

bool StaNetworkMsgHandlerEnable(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool noConnect;
    if (!SupplicantStaNetworkParseEnableReq(data, length - 8, noConnect)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <Enable>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <Enable>: (" << noConnect << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <Disable> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <Select> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupMgmtCipherMask groupMgmtCipherMask;
    if (!SupplicantStaNetworkParseSetGroupMgmtCipherReq(data, length - 8, groupMgmtCipherMask)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetGroupMgmtCipher>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetGroupMgmtCipher>: (" << toString(groupMgmtCipherMask) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <GetGroupMgmtCipher> for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupMgmtCipherMask mask;
    ndk::ScopedAStatus status = stanetwork_instance->getGroupMgmtCipher(&mask);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    LOG(INFO) << "Sending <GetGroupMgmtCipher> resp: (" << toString(mask) << ")";

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetGroupMgmtCipherCfm(param, mask, outData);
}

bool StaNetworkMsgHandlerSetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    if (!data || length < 8) {
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePassword;
    if (!SupplicantStaNetworkParseSetSaePasswordReq(data, length - 8, saePassword)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetSaePassword>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetSaePassword>: (" << saePassword << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> serializedEntry;
    if (!SupplicantStaNetworkParseSetPmkCacheReq(data, length - 8, serializedEntry)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetPmkCache>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    Vector2String(serializedEntry, temp);
    LOG(INFO) << "Processing StaNetwork Req <SetPmkCache>: (" << temp << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
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
        LOG(ERROR) << "Invalid data payload length " << std::to_string(length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    int32_t ifId = CONVERT_INT32_FROM_VECTOR(data, length - 8);
    int32_t networkId = CONVERT_INT32_FROM_VECTOR(data, length - 4);
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifId, &ifName)){
        LOG(ERROR) << "[Fail] Interface instance id" << ifId << "not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SaeH2eMode mode;
    if (!SupplicantStaNetworkParseSetSaeH2eModeReq(data, length - 8, mode)) {
        LOG(ERROR) << "[Fail] Parsing StaNetwork Req <SetSaeH2eMode>";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    LOG(INFO) << "Processing StaNetwork Req <SetSaeH2eMode>: (" << toString(mode) << ") for network -> " << networkId <<" of iface --> "<< ifName;

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        LOG(ERROR) << "[Fail] Network not found";
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setSaeH2eMode(mode);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}
