/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_sta_network_msg.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "SupplicantStaNetworkReq.h"

#include "supplicant_someip_common.h"
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

bool StaNetworkMsgHandlerDisable(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerEnable(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool noConnect;
    if (!SupplicantStaNetworkParseEnableReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, noConnect)) {
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

bool StaNetworkMsgHandlerEnableSaePkOnlyMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseEnableSaePkOnlyModeReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <EnableSaePkOnlyMode>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <EnableSaePkOnlyMode>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->enableSaePkOnlyMode(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerEnableSuiteBEapOpenSslCiphers(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <EnableSuiteBEapOpenSslCiphers> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->enableSuiteBEapOpenSslCiphers();
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerEnableTlsSuiteBEapPhase1Param(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseEnableTlsSuiteBEapPhase1ParamReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <EnableTlsSuiteBEapPhase1Param>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <EnableTlsSuiteBEapPhase1Param>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->enableTlsSuiteBEapPhase1Param(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerGetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetEapAltSubjectMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapAltSubjectMatch> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string altSubjectMatch;
    ndk::ScopedAStatus status = stanetwork_instance->getEapAltSubjectMatch(&altSubjectMatch);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapAltSubjectMatch> resp: (%s)", altSubjectMatch.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapAltSubjectMatchCfm(param, altSubjectMatch, outData);
}

bool StaNetworkMsgHandlerGetEapAnonymousIdentity(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapAnonymousIdentity> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> anonymousIdentity;
    ndk::ScopedAStatus status = stanetwork_instance->getEapAnonymousIdentity(&anonymousIdentity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(anonymousIdentity, temp);
    ALOGI("Sending <GetEapAnonymousIdentity> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapAnonymousIdentityCfm(param, anonymousIdentity, outData);
}

bool StaNetworkMsgHandlerGetEapCACert(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapCACert> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string caCert;
    ndk::ScopedAStatus status = stanetwork_instance->getEapCACert(&caCert);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapCACert> resp: (%s)", caCert.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapCACertCfm(param, caCert, outData);
}

bool StaNetworkMsgHandlerGetEapCAPath(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapCAPath> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string caPath;
    ndk::ScopedAStatus status = stanetwork_instance->getEapCAPath(&caPath);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapCAPath> resp: (%s)", caPath.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapCAPathCfm(param, caPath, outData);
}

bool StaNetworkMsgHandlerGetEapClientCert(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapClientCert> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string clientCert;
    ndk::ScopedAStatus status = stanetwork_instance->getEapClientCert(&clientCert);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapClientCert> resp: (%s)", clientCert.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapClientCertCfm(param, clientCert, outData);
}

bool StaNetworkMsgHandlerGetEapDomainSuffixMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapDomainSuffixMatch> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string domainSuffixMatch;
    ndk::ScopedAStatus status = stanetwork_instance->getEapDomainSuffixMatch(&domainSuffixMatch);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapDomainSuffixMatch> resp: (%s)", domainSuffixMatch.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapDomainSuffixMatchCfm(param, domainSuffixMatch, outData);
}

bool StaNetworkMsgHandlerGetEapEngine(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapEngine> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getEapEngine(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapEngine> resp: (%d)", res);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapEngineCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetEapEngineId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapEngineId> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string engineId;
    ndk::ScopedAStatus status = stanetwork_instance->getEapEngineId(&engineId);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapEngineId> resp: (%s)", engineId.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapEngineIdCfm(param, engineId, outData);
}

bool StaNetworkMsgHandlerGetEapIdentity(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapIdentity> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> eapIdentity;
    ndk::ScopedAStatus status = stanetwork_instance->getEapIdentity(&eapIdentity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(eapIdentity, temp);
    ALOGI("Sending <GetEapIdentity> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapIdentityCfm(param, eapIdentity, outData);
}

bool StaNetworkMsgHandlerGetEapMethod(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapMethod> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    EapMethod method;
    ndk::ScopedAStatus status = stanetwork_instance->getEapMethod(&method);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapMethod> resp: (%s)", toString(method).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapMethodCfm(param, method, outData);
}

bool StaNetworkMsgHandlerGetEapPassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapPassword> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> eapPassword;
    ndk::ScopedAStatus status = stanetwork_instance->getEapPassword(&eapPassword);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(eapPassword, temp);
    ALOGI("Sending <GetEapPassword> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapPasswordCfm(param, eapPassword, outData);
}

bool StaNetworkMsgHandlerGetEapPhase2Method(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapPhase2Method> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    EapPhase2Method method;
    ndk::ScopedAStatus status = stanetwork_instance->getEapPhase2Method(&method);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapPhase2Method> resp: (%s)", toString(method).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapPhase2MethodCfm(param, method, outData);
}

bool StaNetworkMsgHandlerGetEapPrivateKeyId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapPrivateKeyId> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string privateKeyId;
    ndk::ScopedAStatus status = stanetwork_instance->getEapPrivateKeyId(&privateKeyId);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapPrivateKeyId> resp: (%s)", privateKeyId.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapPrivateKeyIdCfm(param, privateKeyId, outData);
}

bool StaNetworkMsgHandlerGetEapSubjectMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEapSubjectMatch> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string subjectMatch;
    ndk::ScopedAStatus status = stanetwork_instance->getEapSubjectMatch(&subjectMatch);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEapSubjectMatch> resp: (%s)", subjectMatch.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEapSubjectMatchCfm(param, subjectMatch, outData);
}

bool StaNetworkMsgHandlerGetEdmg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetEdmg> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool res;
    ndk::ScopedAStatus status = stanetwork_instance->getEdmg(&res);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetEdmg> resp: (%d)", res);

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetEdmgCfm(param, res, outData);
}

bool StaNetworkMsgHandlerGetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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


bool StaNetworkMsgHandlerGetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetInterfaceName(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetInterfaceName> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string interfaceName;
    ndk::ScopedAStatus status = stanetwork_instance->getInterfaceName(&interfaceName);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetInterfaceName> resp: (%s)", interfaceName.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetInterfaceNameCfm(param, interfaceName, outData);
}

bool StaNetworkMsgHandlerGetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetOcsp(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetOcsp> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    OcspType type;
    ndk::ScopedAStatus status = stanetwork_instance->getOcsp(&type);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetOcsp> resp: (%s)", toString(type).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetOcspCfm(param, type, outData);
}

bool StaNetworkMsgHandlerGetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetSaePasswordId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetSaePasswordId> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePwId;
    ndk::ScopedAStatus status = stanetwork_instance->getSaePasswordId(&saePwId);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetSaePasswordId> resp: (%s)", saePwId.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetSaePasswordIdCfm(param, saePwId, outData);
}

bool StaNetworkMsgHandlerGetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetType(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetType> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    IfaceType type;
    ndk::ScopedAStatus status = stanetwork_instance->getType(&type);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetType> resp: (%s)", toString(type).c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetTypeCfm(param, type, outData);
}

bool StaNetworkMsgHandlerGetWapiCertSuite(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetWapiCertSuite> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string wapiCertSuite;
    ndk::ScopedAStatus status = stanetwork_instance->getWapiCertSuite(&wapiCertSuite);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    ALOGI("Sending <GetWapiCertSuite> resp: (%s)", wapiCertSuite.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWapiCertSuiteCfm(param, wapiCertSuite, outData);
}

bool StaNetworkMsgHandlerGetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseGetWepKeyReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, keyIdx)) {
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
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerGetWpsNfcConfigurationToken(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <GetWpsNfcConfigurationToken> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> token;
    ndk::ScopedAStatus status = stanetwork_instance->getWpsNfcConfigurationToken(&token);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    std::string temp;
    v2s(token, temp);
    ALOGI("Sending <GetWpsNfcConfigurationToken> resp: (%s)", temp.c_str());

    HalStatusParam param;
    ScopedAStatus2HalStatusParam(status, param);
    return SupplicantStaNetworkSerializeGetWpsNfcConfigurationTokenCfm(param, token, outData);
}

bool StaNetworkMsgHandlerSelect(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
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

bool StaNetworkMsgHandlerSendNetworkEapIdentityResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SendNetworkEapIdentityResponseReqStaNetworkParam buff;
    if (!SupplicantStaNetworkParseSendNetworkEapIdentityResponseReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, buff)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SendNetworkEapIdentityResponse>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SendNetworkEapIdentityResponse> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapIdentityResponse(buff.identity, buff.encryptedIdentity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSendNetworkEapSimGsmAuthFailure(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SendNetworkEapSimGsmAuthFailure> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapSimGsmAuthFailure();
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSendNetworkEapSimGsmAuthResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<NetworkResponseEapSimGsmAuthParams> params;
    if (!SupplicantStaNetworkParseSendNetworkEapSimGsmAuthResponseReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, params)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SendNetworkEapSimGsmAuthResponse>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SendNetworkEapSimGsmAuthResponse> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapSimGsmAuthResponse(params);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSendNetworkEapSimUmtsAuthFailure(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SendNetworkEapSimUmtsAuthFailure> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapSimUmtsAuthFailure();
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSendNetworkEapSimUmtsAuthResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    NetworkResponseEapSimUmtsAuthParams params;
    if (!SupplicantStaNetworkParseSendNetworkEapSimUmtsAuthResponseReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, params)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SendNetworkEapSimUmtsAuthResponse>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SendNetworkEapSimUmtsAuthResponse> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapSimUmtsAuthResponse(params);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSendNetworkEapSimUmtsAutsResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> auts;
    if (!SupplicantStaNetworkParseSendNetworkEapSimUmtsAutsResponseReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, auts)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SendNetworkEapSimUmtsAutsResponse>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(auts, temp);
    ALOGI("Processing StaNetwork Req <SendNetworkEapSimUmtsAutsResponse>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->sendNetworkEapSimUmtsAutsResponse(auts);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    AuthAlgMask authAlgMask;
    if (!SupplicantStaNetworkParseSetAuthAlgReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, authAlgMask)) {
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

bool StaNetworkMsgHandlerSetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> bssid;
    if (!SupplicantStaNetworkParseSetBssidReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, bssid)) {
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

bool StaNetworkMsgHandlerSetDppKeys(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    DppConnectionKeys keys;
    if (!SupplicantStaNetworkParseSetDppKeysReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, keys)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetDppKeys>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetDppKeys> for network -> %d of iface --> %s", networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setDppKeys(keys);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapAltSubjectMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string match;
    if (!SupplicantStaNetworkParseSetEapAltSubjectMatchReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, match)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapAltSubjectMatch>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapAltSubjectMatch>: (%s) for network -> %d of iface --> %s", match.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapAltSubjectMatch(match);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapAnonymousIdentity(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> identity;
    if (!SupplicantStaNetworkParseSetEapAnonymousIdentityReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, identity)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapAnonymousIdentity>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(identity, temp);
    ALOGI("Processing StaNetwork Req <SetEapAnonymousIdentity>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapAnonymousIdentity(identity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapCACert(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string path;
    if (!SupplicantStaNetworkParseSetEapCACertReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, path)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapCACert>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string local_path = "/data/vendor/wifi/wpa/cacert.pem";

    ALOGI("Processing StaNetwork Req <SetEapCACert>: (%s) for network -> %d of iface --> %s", path.c_str(), networkId, ifName.c_str());
    ALOGI("Overwrite CACert with local path: (%s)", local_path.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapCACert(local_path);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapCAPath(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string path;
    if (!SupplicantStaNetworkParseSetEapCAPathReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, path)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapCAPath>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string local_path = "/data/vendor/wifi/wpa";

    ALOGI("Processing StaNetwork Req <SetEapCAPath>: (%s) for network -> %d of iface --> %s", path.c_str(), networkId, ifName.c_str());
    ALOGI("Overwrite CAPath with local path: (%s)", local_path.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapCAPath(local_path);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapClientCert(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string path;
    if (!SupplicantStaNetworkParseSetEapClientCertReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, path)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapClientCert>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapClientCert>: (%s) for network -> %d of iface --> %s", path.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapClientCert(path);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapDomainSuffixMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string match;
    if (!SupplicantStaNetworkParseSetEapDomainSuffixMatchReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, match)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapDomainSuffixMatch>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapDomainSuffixMatch>: (%s) for network -> %d of iface --> %s", match.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapDomainSuffixMatch(match);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapEncryptedImsiIdentity(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> identity;
    if (!SupplicantStaNetworkParseSetEapEncryptedImsiIdentityReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, identity)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapEncryptedImsiIdentity>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(identity, temp);
    ALOGI("Processing StaNetwork Req <SetEapEncryptedImsiIdentity>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapEncryptedImsiIdentity(identity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapEngine(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetEapEngineReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapEngine>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapEngine>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapEngine(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapEngineID(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string id;
    if (!SupplicantStaNetworkParseSetEapEngineIDReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, id)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapEngineID>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapEngineID>: (%s) for network -> %d of iface --> %s", id.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapEngineID(id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapErp(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetEapErpReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapErp>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapErp>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapErp(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapIdentity(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> identity;
    if (!SupplicantStaNetworkParseSetEapIdentityReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, identity)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapIdentity>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(identity, temp);
    ALOGI("Processing StaNetwork Req <SetEapIdentity>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapIdentity(identity);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapMethod(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    EapMethod method;
    if (!SupplicantStaNetworkParseSetEapMethodReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, method)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapMethod>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapMethod>: (%s) for network -> %d of iface --> %s", toString(method).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapMethod(method);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapPassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> password;
    if (!SupplicantStaNetworkParseSetEapPasswordReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, password)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapPassword>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(password, temp);
    ALOGI("Processing StaNetwork Req <SetEapPassword>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapPassword(password);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapPhase2Method(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    EapPhase2Method method;
    if (!SupplicantStaNetworkParseSetEapPhase2MethodReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, method)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapPhase2Method>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapPhase2Method>: (%s) for network -> %d of iface --> %s", toString(method).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapPhase2Method(method);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapPrivateKeyId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string id;
    if (!SupplicantStaNetworkParseSetEapPrivateKeyIdReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, id)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapPrivateKeyId>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapPrivateKeyId>: (%s) for network -> %d of iface --> %s", id.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapPrivateKeyId(id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEapSubjectMatch(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string match;
    if (!SupplicantStaNetworkParseSetEapSubjectMatchReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, match)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEapSubjectMatch>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEapSubjectMatch>: (%s) for network -> %d of iface --> %s", match.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEapSubjectMatch(match);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetEdmg(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetEdmgReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetEdmg>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetEdmg>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setEdmg(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupCipherMask groupCipherMask;
    if (!SupplicantStaNetworkParseSetGroupCipherReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, groupCipherMask)) {
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

bool StaNetworkMsgHandlerSetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    GroupMgmtCipherMask groupMgmtCipherMask;
    if (!SupplicantStaNetworkParseSetGroupMgmtCipherReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, groupMgmtCipherMask)) {
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

bool StaNetworkMsgHandlerSetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string idStr;
    if (!SupplicantStaNetworkParseSetIdStrReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, idStr)) {
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

bool StaNetworkMsgHandlerSetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    KeyMgmtMask keyMgmtMask;
    if (!SupplicantStaNetworkParseSetKeyMgmtReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, keyMgmtMask)) {
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

bool StaNetworkMsgHandlerSetOcsp(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    OcspType ocspType;
    if (!SupplicantStaNetworkParseSetOcspReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, ocspType)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetOcsp>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetOcsp>: (%s) for network -> %d of iface --> %s", toString(ocspType).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setOcsp(ocspType);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    PairwiseCipherMask pairwiseCipherMask;
    if (!SupplicantStaNetworkParseSetPairwiseCipherReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, pairwiseCipherMask)) {
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

bool StaNetworkMsgHandlerSetPmkCache(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> serializedEntry;
    if (!SupplicantStaNetworkParseSetPmkCacheReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, serializedEntry)) {
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

bool StaNetworkMsgHandlerSetProactiveKeyCaching(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetProactiveKeyCachingReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetProactiveKeyCaching>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetProactiveKeyCaching>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setProactiveKeyCaching(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ProtoMask protoMask;
    if (!SupplicantStaNetworkParseSetProtoReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, protoMask)) {
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

bool StaNetworkMsgHandlerSetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> psk;
    if (!SupplicantStaNetworkParseSetPskReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, psk)) {
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

bool StaNetworkMsgHandlerSetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string psk;
    if (!SupplicantStaNetworkParseSetPskPassphraseReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, psk)) {
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

bool StaNetworkMsgHandlerSetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetRequirePmfReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
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

bool StaNetworkMsgHandlerSetSaeH2eMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SaeH2eMode mode;
    if (!SupplicantStaNetworkParseSetSaeH2eModeReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, mode)) {
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

bool StaNetworkMsgHandlerSetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePassword;
    if (!SupplicantStaNetworkParseSetSaePasswordReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, saePassword)) {
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

bool StaNetworkMsgHandlerSetSaePasswordId(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string saePasswordID;
    if (!SupplicantStaNetworkParseSetSaePasswordIdReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, saePasswordID)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetSaePasswordId>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetSaePasswordId>: (%s) for network -> %d of iface --> %s", saePasswordID.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setSaePasswordId(saePasswordID);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetScanSsidReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
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

bool StaNetworkMsgHandlerSetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> ssid;
    if (!SupplicantStaNetworkParseSetSsidReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, ssid)) {
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

bool StaNetworkMsgHandlerSetUpdateIdentifier(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t id;
    if (!SupplicantStaNetworkParseSetUpdateIdentifierReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, id)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetUpdateIdentifier>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetUpdateIdentifier>: (%d) for network -> %d of iface --> %s", id, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setUpdateIdentifier(id);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetWapiCertSuite(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string suite;
    if (!SupplicantStaNetworkParseSetWapiCertSuiteReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, suite)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetWapiCertSuite>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetWapiCertSuite>: (%s) for network -> %d of iface --> %s", suite.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setWapiCertSuite(suite);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    SetWepKeyReqStaNetworkParam buff;
    if (!SupplicantStaNetworkParseSetWepKeyReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, buff)) {
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
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    int32_t keyIdx;
    if (!SupplicantStaNetworkParseSetWepTxKeyIdxReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, keyIdx)) {
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

bool StaNetworkMsgHandlerSetRoamingConsortiumSelection(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::vector<uint8_t> selectedRcoi;
    if (!SupplicantStaNetworkParseSetRoamingConsortiumSelectionReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, selectedRcoi)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetRoamingConsortiumSelection>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string temp;
    v2s(selectedRcoi, temp);
    ALOGI("Processing StaNetwork Req <SetRoamingConsortiumSelection>: (%s) for network -> %d of iface --> %s", temp.c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setRoamingConsortiumSelection(selectedRcoi);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetMinimumTlsVersionEapPhase1Param(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    TlsVersion tlsVersion;
    if (!SupplicantStaNetworkParseSetMinimumTlsVersionEapPhase1ParamReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, tlsVersion)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetMinimumTlsVersionEapPhase1Param>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetMinimumTlsVersionEapPhase1Param>: (%s) for network -> %d of iface --> %s", toString(tlsVersion).c_str(), networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setMinimumTlsVersionEapPhase1Param(tlsVersion);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}

bool StaNetworkMsgHandlerSetStrictConservativePeerMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData)
{
    uint16_t ifaceId, networkId;
    if (!SupplicantParseInstanceId(data, length, ifaceId, networkId)) {
        ALOGE("Invalid data payload length: %zu", length);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    std::string ifName;
    AidlManager *aidl_manager = AidlManager::getInstance();
    if(aidl_manager->getStaIfaceNameByInstanceId(ifaceId, &ifName)){
        ALOGE("[Fail] Interface instance id (%d) not found", ifaceId);
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    bool enable;
    if (!SupplicantStaNetworkParseSetStrictConservativePeerModeReq(data + SUPPLICANT_PAYLOAD_MIN_SIZE, length - SUPPLICANT_PAYLOAD_MIN_SIZE, enable)) {
        ALOGE("[Fail] Parsing StaNetwork Req <SetStrictConservativePeerMode>");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_ARGS_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ALOGI("Processing StaNetwork Req <SetStrictConservativePeerMode>: (%d) for network -> %d of iface --> %s", enable, networkId, ifName.c_str());

    std::shared_ptr<ISupplicantStaNetwork> stanetwork_instance;
    if(aidl_manager->getStaNetworkAidlObjectByIfnameAndNetworkId(ifName, networkId, &stanetwork_instance)) {
        ALOGE("[Fail] Network not found");
        ndk::ScopedAStatus status(SupplicantStatusCode::FAILURE_NETWORK_INVALID);
        return StaNetworkSerializeStatus(status, outData);
    }

    ndk::ScopedAStatus status = stanetwork_instance->setStrictConservativePeerMode(enable);
    SUPPLICANT_STANETWORK_PRINT_CFM_STATUS(__func__, status);

    return StaNetworkSerializeStatus(status, outData);
}
