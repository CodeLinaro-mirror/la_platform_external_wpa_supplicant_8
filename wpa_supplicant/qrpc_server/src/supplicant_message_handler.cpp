/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <map>
#include <functional>

#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>
#ifdef CONFIG_USE_VENDOR_AIDL
#include <rpc/message/wpa_supplicant_vendor/supplicant_vendor_message_def.h>
#endif

#include "supplicant_message_handler.h"

#include "SupplicantReq.h"
#include "SupplicantStaIfaceReq.h"
#include "SupplicantStaNetworkReq.h"
#ifdef CONFIG_USE_VENDOR_AIDL
#include "SupplicantVendorReq.h"
#include "SupplicantVendorStaIfaceReq.h"
#endif

typedef std::function< bool (uint8_t* data, size_t length, std::vector<uint8_t>& outData) > MessageHandler;
static std::map<uint16_t, MessageHandler> msgHandlerMap = {
    /* ISupplicant */
//    {SUPPLICANT_ADD_P2P_INTERFACE_REQ},
    {SUPPLICANT_ADD_STA_INTERFACE_REQ, &SupplicantMsgHandlerAddStaInterface},
    {SUPPLICANT_GET_DEBUG_LEVEL_REQ, &SupplicantMsgHandlerGetDebugLevel},
//    {SUPPLICANT_GET_P2P_INTERFACE_REQ},
//  {SUPPLICANT_GET_STA_INTERFACE_REQ},
    {SUPPLICANT_IS_DEBUG_SHOW_KEYS_ENABLED_REQ, &SupplicantMsgHandlerIsDebugShowKeysEnabled},
    {SUPPLICANT_IS_DEBUG_SHOW_TIMESTAMP_ENABLED_REQ, &SupplicantMsgHandlerIsDebugShowTimestampEnabled},
    {SUPPLICANT_LIST_INTERFACES_REQ, &SupplicantMsgHandlerListInterfaces},
//    {SUPPLICANT_REGISTER_CALLBACK_REQ},
    {SUPPLICANT_REMOVE_INTERFACE_REQ, &SupplicantMsgHandlerRemoveInterface},
    {SUPPLICANT_SET_CONCURRENCY_PRIORITY_REQ, &SupplicantMsgHandlerSetConcurrencyPriority},
    {SUPPLICANT_SET_DEBUG_PARAMS_REQ, &SupplicantMsgHandlerSetDebugParams},
    {SUPPLICANT_TERMINATE_REQ, &SupplicantMsgHandlerTerminate},
//    {SUPPLICANT_REGISTER_NON_STANDARD_CERT_CALLBACK_REQ},
#ifdef CONFIG_USE_VENDOR_AIDL
    /* ISupplicantVendor */
    {SUPPLICANT_VENDOR_LIST_VENDOR_INTERFACES_REQ, &SupplicantVendorMsgHandlerListVendorInterfaces},
#endif
    /* ISupplicantStaIface */
    {SUPPLICANT_STA_IFACE_ADD_DPP_PEER_URI_REQ, &StaIfaceMsgHandlerAddDppPeerUri},
    {SUPPLICANT_STA_IFACE_ADD_EXT_RADIO_WORK_REQ, &StaIfaceMsgHandlerAddExtRadioWork},
    {SUPPLICANT_STA_IFACE_ADD_NETWORK_REQ, &StaIfaceMsgHandlerAddNetwork},
    {SUPPLICANT_STA_IFACE_ADD_RX_FILTER_REQ, &StaIfaceMsgHandlerAddRxFilter},
    {SUPPLICANT_STA_IFACE_CANCEL_WPS_REQ, &StaIfaceMsgHandlerCancelWps},
    {SUPPLICANT_STA_IFACE_DISCONNECT_REQ, &StaIfaceMsgHandlerDisconnect},
    {SUPPLICANT_STA_IFACE_ENABLE_AUTO_RECONNECT_REQ, &StaIfaceMsgHandlerEnableAutoReconnect},
    {SUPPLICANT_STA_IFACE_FILS_HLP_ADD_REQUEST_REQ, &StaIfaceMsgHandlerFilsHlpAddRequest},
    {SUPPLICANT_STA_IFACE_FILS_HLP_FLUSH_REQUEST_REQ, &StaIfaceMsgHandlerFilsHlpFlushRequest},
    {SUPPLICANT_STA_IFACE_GENERATE_DPP_BOOTSTRAP_INFO_FOR_RESPONDER_REQ, &StaIfaceMsgHandlerGenerateDppBootstrapInfoForResponder},
    {SUPPLICANT_STA_IFACE_GENERATE_SELF_DPP_CONFIGURATION_REQ, &StaIfaceMsgHandlerGenerateSelfDppConfiguration},
    {SUPPLICANT_STA_IFACE_GET_CONNECTION_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetConnectionCapabilities},
    {SUPPLICANT_STA_IFACE_GET_CONNECTION_MLO_LINKS_INFO_REQ, &StaIfaceMsgHandlerGetConnectionMloLinksInfo},
    {SUPPLICANT_STA_IFACE_GET_KEY_MGMT_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetKeyMgmtCapabilities},
    {SUPPLICANT_STA_IFACE_GET_MAC_ADDRESS_REQ, &StaIfaceMsgHandlerGetMacAddress},
    {SUPPLICANT_STA_IFACE_GET_NAME_REQ, &StaIfaceMsgHandlerGetName},
//    {SUPPLICANT_STA_IFACE_GET_NETWORK_REQ},
    {SUPPLICANT_STA_IFACE_GET_TYPE_REQ, &StaIfaceMsgHandlerGetType},
    {SUPPLICANT_STA_IFACE_GET_WPA_DRIVER_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetWpaDriverCapabilities},
    {SUPPLICANT_STA_IFACE_INITIATE_ANQP_QUERY_REQ, &StaIfaceMsgHandlerInitiateAnqpQuery},
    {SUPPLICANT_STA_IFACE_INITIATE_HS20_ICON_QUERY_REQ, &StaIfaceMsgHandlerInitiateHs20IconQuery},
    {SUPPLICANT_STA_IFACE_INITIATE_TDLS_DISCOVER_REQ, &StaIfaceMsgHandlerInitiateTdlsDiscover},
    {SUPPLICANT_STA_IFACE_INITIATE_TDLS_SETUP_REQ, &StaIfaceMsgHandlerInitiateTdlsSetup},
    {SUPPLICANT_STA_IFACE_INITIATE_TDLS_TEARDOWN_REQ, &StaIfaceMsgHandlerInitiateTdlsTeardown},
    {SUPPLICANT_STA_IFACE_INITIATE_VENUE_URL_ANQP_QUERY_REQ, &StaIfaceMsgHandlerInitiateVenueUrlAnqpQuery},
    {SUPPLICANT_STA_IFACE_LIST_NETWORKS_REQ, &StaIfaceMsgHandlerListNetworks},
    {SUPPLICANT_STA_IFACE_REASSOCIATE_REQ, &StaIfaceMsgHandlerReassociate},
    {SUPPLICANT_STA_IFACE_RECONNECT_REQ, &StaIfaceMsgHandlerReconnect},
//    {SUPPLICANT_STA_IFACE_REGISTER_CALLBACK_REQ},
    {SUPPLICANT_STA_IFACE_SET_QOS_POLICY_FEATURE_ENABLED_REQ, &StaIfaceMsgHandlerSetQosPolicyFeatureEnabled},
    {SUPPLICANT_STA_IFACE_SEND_QOS_POLICY_RESPONSE_REQ, &StaIfaceMsgHandlerSendQosPolicyResponse},
    {SUPPLICANT_STA_IFACE_REMOVE_ALL_QOS_POLICIES_REQ, &StaIfaceMsgHandlerRemoveAllQosPolicies},
    {SUPPLICANT_STA_IFACE_REMOVE_DPP_URI_REQ, &StaIfaceMsgHandlerRemoveDppUri},
    {SUPPLICANT_STA_IFACE_REMOVE_EXT_RADIO_WORK_REQ, &StaIfaceMsgHandlerRemoveExtRadioWork},
    {SUPPLICANT_STA_IFACE_REMOVE_NETWORK_REQ, &StaIfaceMsgHandlerRemoveNetwork},
    {SUPPLICANT_STA_IFACE_REMOVE_RX_FILTER_REQ, &StaIfaceMsgHandlerRemoveRxFilter},
    {SUPPLICANT_STA_IFACE_SET_BT_COEXISTENCE_MODE_REQ, &StaIfaceMsgHandlerSetBtCoexistenceMode},
    {SUPPLICANT_STA_IFACE_SET_BT_COEXISTENCE_SCAN_MODE_ENABLED_REQ, &StaIfaceMsgHandlerSetBtCoexistenceScanModeEnabled},
    {SUPPLICANT_STA_IFACE_SET_COUNTRY_CODE_REQ, &StaIfaceMsgHandlerSetCountryCode},
    {SUPPLICANT_STA_IFACE_SET_EXTERNAL_SIM_REQ, &StaIfaceMsgHandlerSetExternalSim},
    {SUPPLICANT_STA_IFACE_SET_MBO_CELLULAR_DATA_STATUS_REQ, &StaIfaceMsgHandlerSetMboCellularDataStatus},
    {SUPPLICANT_STA_IFACE_SET_POWER_SAVE_REQ, &StaIfaceMsgHandlerSetPowerSave},
    {SUPPLICANT_STA_IFACE_SET_SUSPEND_MODE_ENABLED_REQ, &StaIfaceMsgHandlerSetSuspendModeEnabled},
    {SUPPLICANT_STA_IFACE_SET_WPS_CONFIG_METHODS_REQ, &StaIfaceMsgHandlerSetWpsConfigMethods},
    {SUPPLICANT_STA_IFACE_SET_WPS_DEVICE_NAME_REQ, &StaIfaceMsgHandlerSetWpsDeviceName},
    {SUPPLICANT_STA_IFACE_SET_WPS_DEVICE_TYPE_REQ, &StaIfaceMsgHandlerSetWpsDeviceType},
    {SUPPLICANT_STA_IFACE_SET_WPS_MANUFACTURER_REQ, &StaIfaceMsgHandlerSetWpsManufacturer},
    {SUPPLICANT_STA_IFACE_SET_WPS_MODEL_NAME_REQ, &StaIfaceMsgHandlerSetWpsModelName},
    {SUPPLICANT_STA_IFACE_SET_WPS_MODEL_NUMBER_REQ, &StaIfaceMsgHandlerSetWpsModelNumber},
    {SUPPLICANT_STA_IFACE_SET_WPS_SERIAL_NUMBER_REQ, &StaIfaceMsgHandlerSetWpsSerialNumber},
    {SUPPLICANT_STA_IFACE_START_DPP_CONFIGURATOR_INITIATOR_REQ, &StaIfaceMsgHandlerStartDppConfiguratorInitiator},
    {SUPPLICANT_STA_IFACE_START_DPP_ENROLLEE_INITIATOR_REQ, &StaIfaceMsgHandlerStartDppEnrolleeInitiator},
    {SUPPLICANT_STA_IFACE_START_DPP_ENROLLEE_RESPONDER_REQ, &StaIfaceMsgHandlerStartDppEnrolleeResponder},
    {SUPPLICANT_STA_IFACE_START_RX_FILTER_REQ, &StaIfaceMsgHandlerStartRxFilter},
    {SUPPLICANT_STA_IFACE_START_WPS_PBC_REQ, &StaIfaceMsgHandlerStartWpsPbc},
    {SUPPLICANT_STA_IFACE_START_WPS_PIN_DISPLAY_REQ, &StaIfaceMsgHandlerStartWpsPinDisplay},
    {SUPPLICANT_STA_IFACE_START_WPS_PIN_KEYPAD_REQ, &StaIfaceMsgHandlerStartWpsPinKeypad},
    {SUPPLICANT_STA_IFACE_START_WPS_REGISTRAR_REQ, &StaIfaceMsgHandlerStartWpsRegistrar},
    {SUPPLICANT_STA_IFACE_STOP_DPP_INITIATOR_REQ, &StaIfaceMsgHandlerStopDppInitiator},
    {SUPPLICANT_STA_IFACE_STOP_DPP_RESPONDER_REQ, &StaIfaceMsgHandlerStopDppResponder},
    {SUPPLICANT_STA_IFACE_STOP_RX_FILTER_REQ, &StaIfaceMsgHandlerStopRxFilter},
    {SUPPLICANT_STA_IFACE_GET_SIGNAL_POLL_RESULTS_REQ, &StaIfaceMsgHandlerGetSignalPollResults},
    {SUPPLICANT_STA_IFACE_ADD_QOS_POLICY_REQUEST_FOR_SCS_REQ, &StaIfaceMsgHandlerAddQosPolicyRequestForScs},
    {SUPPLICANT_STA_IFACE_REMOVE_QOS_POLICY_FOR_SCS_REQ, &StaIfaceMsgHandlerRemoveQosPolicyForScs},
#ifdef CONFIG_USE_VENDOR_AIDL
    /* ISupplicantVendorStaIface */
    {SUPPLICANT_VENDOR_STA_IFACE_DO_DRIVER_CMD_REQ, &VendorStaIfaceMsgHandlerDoDriverCmd},
#endif
    /* ISupplicantStaNetwork */
    {SUPPLICANT_STA_NETWORK_DISABLE_REQ, &StaNetworkMsgHandlerDisable},
    {SUPPLICANT_STA_NETWORK_ENABLE_REQ, &StaNetworkMsgHandlerEnable},
    {SUPPLICANT_STA_NETWORK_ENABLE_SAE_PK_ONLY_MODE_REQ, &StaNetworkMsgHandlerEnableSaePkOnlyMode},
    {SUPPLICANT_STA_NETWORK_ENABLE_SUITE_B_EAP_OPEN_SSL_CIPHERS_REQ, &StaNetworkMsgHandlerEnableSuiteBEapOpenSslCiphers},
    {SUPPLICANT_STA_NETWORK_ENABLE_TLS_SUITE_B_EAP_PHASE1_PARAM_REQ, &StaNetworkMsgHandlerEnableTlsSuiteBEapPhase1Param},
    {SUPPLICANT_STA_NETWORK_GET_AUTH_ALG_REQ, &StaNetworkMsgHandlerGetAuthAlg},
    {SUPPLICANT_STA_NETWORK_GET_BSSID_REQ, &StaNetworkMsgHandlerGetBssid},
    {SUPPLICANT_STA_NETWORK_GET_EAP_ALT_SUBJECT_MATCH_REQ, &StaNetworkMsgHandlerGetEapAltSubjectMatch},
    {SUPPLICANT_STA_NETWORK_GET_EAP_ANONYMOUS_IDENTITY_REQ, &StaNetworkMsgHandlerGetEapAnonymousIdentity},
    {SUPPLICANT_STA_NETWORK_GET_EAP_C_A_CERT_REQ, &StaNetworkMsgHandlerGetEapCACert},
    {SUPPLICANT_STA_NETWORK_GET_EAP_C_A_PATH_REQ, &StaNetworkMsgHandlerGetEapCAPath},
    {SUPPLICANT_STA_NETWORK_GET_EAP_CLIENT_CERT_REQ, &StaNetworkMsgHandlerGetEapClientCert},
    {SUPPLICANT_STA_NETWORK_GET_EAP_DOMAIN_SUFFIX_MATCH_REQ, &StaNetworkMsgHandlerGetEapDomainSuffixMatch},
    {SUPPLICANT_STA_NETWORK_GET_EAP_ENGINE_REQ, &StaNetworkMsgHandlerGetEapEngine},
    {SUPPLICANT_STA_NETWORK_GET_EAP_ENGINE_ID_REQ, &StaNetworkMsgHandlerGetEapEngineId},
    {SUPPLICANT_STA_NETWORK_GET_EAP_IDENTITY_REQ, &StaNetworkMsgHandlerGetEapIdentity},
    {SUPPLICANT_STA_NETWORK_GET_EAP_METHOD_REQ, &StaNetworkMsgHandlerGetEapMethod},
    {SUPPLICANT_STA_NETWORK_GET_EAP_PASSWORD_REQ, &StaNetworkMsgHandlerGetEapPassword},
    {SUPPLICANT_STA_NETWORK_GET_EAP_PHASE2_METHOD_REQ, &StaNetworkMsgHandlerGetEapPhase2Method},
    {SUPPLICANT_STA_NETWORK_GET_EAP_PRIVATE_KEY_ID_REQ, &StaNetworkMsgHandlerGetEapPrivateKeyId},
    {SUPPLICANT_STA_NETWORK_GET_EAP_SUBJECT_MATCH_REQ, &StaNetworkMsgHandlerGetEapSubjectMatch},
    {SUPPLICANT_STA_NETWORK_GET_EDMG_REQ, &StaNetworkMsgHandlerGetEdmg},
    {SUPPLICANT_STA_NETWORK_GET_GROUP_CIPHER_REQ, &StaNetworkMsgHandlerGetGroupCipher},
    {SUPPLICANT_STA_NETWORK_GET_GROUP_MGMT_CIPHER_REQ, &StaNetworkMsgHandlerGetGroupMgmtCipher},
    {SUPPLICANT_STA_NETWORK_GET_ID_REQ, &StaNetworkMsgHandlerGetId},
    {SUPPLICANT_STA_NETWORK_GET_ID_STR_REQ, &StaNetworkMsgHandlerGetIdStr},
    {SUPPLICANT_STA_NETWORK_GET_INTERFACE_NAME_REQ, &StaNetworkMsgHandlerGetInterfaceName},
    {SUPPLICANT_STA_NETWORK_GET_KEY_MGMT_REQ, &StaNetworkMsgHandlerGetKeyMgmt},
    {SUPPLICANT_STA_NETWORK_GET_OCSP_REQ, &StaNetworkMsgHandlerGetOcsp},
    {SUPPLICANT_STA_NETWORK_GET_PAIRWISE_CIPHER_REQ, &StaNetworkMsgHandlerGetPairwiseCipher},
    {SUPPLICANT_STA_NETWORK_GET_PROTO_REQ, &StaNetworkMsgHandlerGetProto},
    {SUPPLICANT_STA_NETWORK_GET_PSK_REQ, &StaNetworkMsgHandlerGetPsk},
    {SUPPLICANT_STA_NETWORK_GET_PSK_PASSPHRASE_REQ, &StaNetworkMsgHandlerGetPskPassphrase},
    {SUPPLICANT_STA_NETWORK_GET_REQUIRE_PMF_REQ, &StaNetworkMsgHandlerGetRequirePmf},
    {SUPPLICANT_STA_NETWORK_GET_SAE_PASSWORD_REQ, &StaNetworkMsgHandlerGetSaePassword},
    {SUPPLICANT_STA_NETWORK_GET_SAE_PASSWORD_ID_REQ, &StaNetworkMsgHandlerGetSaePasswordId},
    {SUPPLICANT_STA_NETWORK_GET_SCAN_SSID_REQ, &StaNetworkMsgHandlerGetScanSsid},
    {SUPPLICANT_STA_NETWORK_GET_SSID_REQ, &StaNetworkMsgHandlerGetSsid},
    {SUPPLICANT_STA_NETWORK_GET_TYPE_REQ, &StaNetworkMsgHandlerGetType},
    {SUPPLICANT_STA_NETWORK_GET_WAPI_CERT_SUITE_REQ, &StaNetworkMsgHandlerGetWapiCertSuite},
    {SUPPLICANT_STA_NETWORK_GET_WEP_KEY_REQ, &StaNetworkMsgHandlerGetWepKey},
    {SUPPLICANT_STA_NETWORK_GET_WEP_TX_KEY_IDX_REQ, &StaNetworkMsgHandlerGetWepTxKeyIdx},
    {SUPPLICANT_STA_NETWORK_GET_WPS_NFC_CONFIGURATION_TOKEN_REQ, &StaNetworkMsgHandlerGetWpsNfcConfigurationToken},
//    {SUPPLICANT_STA_NETWORK_REGISTER_CALLBACK_REQ},
    {SUPPLICANT_STA_NETWORK_SELECT_REQ, &StaNetworkMsgHandlerSelect},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_IDENTITY_RESPONSE_REQ, &StaNetworkMsgHandlerSendNetworkEapIdentityResponse},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_SIM_GSM_AUTH_FAILURE_REQ, &StaNetworkMsgHandlerSendNetworkEapSimGsmAuthFailure},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_SIM_GSM_AUTH_RESPONSE_REQ, &StaNetworkMsgHandlerSendNetworkEapSimGsmAuthResponse},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_SIM_UMTS_AUTH_FAILURE_REQ, &StaNetworkMsgHandlerSendNetworkEapSimUmtsAuthFailure},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_SIM_UMTS_AUTH_RESPONSE_REQ, &StaNetworkMsgHandlerSendNetworkEapSimUmtsAuthResponse},
    {SUPPLICANT_STA_NETWORK_SEND_NETWORK_EAP_SIM_UMTS_AUTS_RESPONSE_REQ, &StaNetworkMsgHandlerSendNetworkEapSimUmtsAutsResponse},
    {SUPPLICANT_STA_NETWORK_SET_AUTH_ALG_REQ, &StaNetworkMsgHandlerSetAuthAlg},
    {SUPPLICANT_STA_NETWORK_SET_BSSID_REQ, &StaNetworkMsgHandlerSetBssid},
    {SUPPLICANT_STA_NETWORK_SET_DPP_KEYS_REQ, &StaNetworkMsgHandlerSetDppKeys},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ALT_SUBJECT_MATCH_REQ, &StaNetworkMsgHandlerSetEapAltSubjectMatch},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ANONYMOUS_IDENTITY_REQ, &StaNetworkMsgHandlerSetEapAnonymousIdentity},
    {SUPPLICANT_STA_NETWORK_SET_EAP_C_A_CERT_REQ, &StaNetworkMsgHandlerSetEapCACert},
    {SUPPLICANT_STA_NETWORK_SET_EAP_C_A_PATH_REQ, &StaNetworkMsgHandlerSetEapCAPath},
    {SUPPLICANT_STA_NETWORK_SET_EAP_CLIENT_CERT_REQ, &StaNetworkMsgHandlerSetEapClientCert},
    {SUPPLICANT_STA_NETWORK_SET_EAP_DOMAIN_SUFFIX_MATCH_REQ, &StaNetworkMsgHandlerSetEapDomainSuffixMatch},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ENCRYPTED_IMSI_IDENTITY_REQ, &StaNetworkMsgHandlerSetEapEncryptedImsiIdentity},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ENGINE_REQ, &StaNetworkMsgHandlerSetEapEngine},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ENGINE_I_D_REQ, &StaNetworkMsgHandlerSetEapEngineID},
    {SUPPLICANT_STA_NETWORK_SET_EAP_ERP_REQ, &StaNetworkMsgHandlerSetEapErp},
    {SUPPLICANT_STA_NETWORK_SET_EAP_IDENTITY_REQ, &StaNetworkMsgHandlerSetEapIdentity},
    {SUPPLICANT_STA_NETWORK_SET_EAP_METHOD_REQ, &StaNetworkMsgHandlerSetEapMethod},
    {SUPPLICANT_STA_NETWORK_SET_EAP_PASSWORD_REQ, &StaNetworkMsgHandlerSetEapPassword},
    {SUPPLICANT_STA_NETWORK_SET_EAP_PHASE2_METHOD_REQ, &StaNetworkMsgHandlerSetEapPhase2Method},
    {SUPPLICANT_STA_NETWORK_SET_EAP_PRIVATE_KEY_ID_REQ, &StaNetworkMsgHandlerSetEapPrivateKeyId},
    {SUPPLICANT_STA_NETWORK_SET_EAP_SUBJECT_MATCH_REQ, &StaNetworkMsgHandlerSetEapSubjectMatch},
    {SUPPLICANT_STA_NETWORK_SET_EDMG_REQ, &StaNetworkMsgHandlerSetEdmg},
    {SUPPLICANT_STA_NETWORK_SET_GROUP_CIPHER_REQ, &StaNetworkMsgHandlerSetGroupCipher},
    {SUPPLICANT_STA_NETWORK_SET_GROUP_MGMT_CIPHER_REQ, &StaNetworkMsgHandlerSetGroupMgmtCipher},
    {SUPPLICANT_STA_NETWORK_SET_ID_STR_REQ, &StaNetworkMsgHandlerSetIdStr},
    {SUPPLICANT_STA_NETWORK_SET_KEY_MGMT_REQ, &StaNetworkMsgHandlerSetKeyMgmt},
    {SUPPLICANT_STA_NETWORK_SET_OCSP_REQ, &StaNetworkMsgHandlerSetOcsp},
    {SUPPLICANT_STA_NETWORK_SET_PAIRWISE_CIPHER_REQ, &StaNetworkMsgHandlerSetPairwiseCipher},
    {SUPPLICANT_STA_NETWORK_SET_PMK_CACHE_REQ, &StaNetworkMsgHandlerSetPmkCache},
    {SUPPLICANT_STA_NETWORK_SET_PROACTIVE_KEY_CACHING_REQ, &StaNetworkMsgHandlerSetProactiveKeyCaching},
    {SUPPLICANT_STA_NETWORK_SET_PROTO_REQ, &StaNetworkMsgHandlerSetProto},
    {SUPPLICANT_STA_NETWORK_SET_PSK_REQ, &StaNetworkMsgHandlerSetPsk},
    {SUPPLICANT_STA_NETWORK_SET_PSK_PASSPHRASE_REQ, &StaNetworkMsgHandlerSetPskPassphrase},
    {SUPPLICANT_STA_NETWORK_SET_REQUIRE_PMF_REQ, &StaNetworkMsgHandlerSetRequirePmf},
    {SUPPLICANT_STA_NETWORK_SET_SAE_H2E_MODE_REQ, &StaNetworkMsgHandlerSetSaeH2eMode},
    {SUPPLICANT_STA_NETWORK_SET_SAE_PASSWORD_REQ, &StaNetworkMsgHandlerSetSaePassword},
    {SUPPLICANT_STA_NETWORK_SET_SAE_PASSWORD_ID_REQ, &StaNetworkMsgHandlerSetSaePasswordId},
    {SUPPLICANT_STA_NETWORK_SET_SCAN_SSID_REQ, &StaNetworkMsgHandlerSetScanSsid},
    {SUPPLICANT_STA_NETWORK_SET_SSID_REQ, &StaNetworkMsgHandlerSetSsid},
    {SUPPLICANT_STA_NETWORK_SET_UPDATE_IDENTIFIER_REQ, &StaNetworkMsgHandlerSetUpdateIdentifier},
    {SUPPLICANT_STA_NETWORK_SET_WAPI_CERT_SUITE_REQ, &StaNetworkMsgHandlerSetWapiCertSuite},
    {SUPPLICANT_STA_NETWORK_SET_WEP_KEY_REQ, &StaNetworkMsgHandlerSetWepKey},
    {SUPPLICANT_STA_NETWORK_SET_WEP_TX_KEY_IDX_REQ, &StaNetworkMsgHandlerSetWepTxKeyIdx},
    {SUPPLICANT_STA_NETWORK_SET_ROAMING_CONSORTIUM_SELECTION_REQ, &StaNetworkMsgHandlerSetRoamingConsortiumSelection},
    {SUPPLICANT_STA_NETWORK_SET_MINIMUM_TLS_VERSION_EAP_PHASE1_PARAM_REQ, &StaNetworkMsgHandlerSetMinimumTlsVersionEapPhase1Param},
    {SUPPLICANT_STA_NETWORK_SET_STRICT_CONSERVATIVE_PEER_MODE_REQ, &StaNetworkMsgHandlerSetStrictConservativePeerMode}
};

static inline MessageHandler SupplicantGetMessageHandler(uint16_t methodId)
{
    auto item = msgHandlerMap.find(methodId);
    if (item != msgHandlerMap.end())
        return item->second;

    return nullptr;
}

void SupplicantProcessSomeIPRequestMessage(const std::shared_ptr<SomeipMessage> &msg)
{
    uint16_t methodId = msg->getMethodId();

    ALOGI("Recv Someip Request message with method_id 0x%04X", methodId);
    MessageHandler handler = SupplicantGetMessageHandler(methodId);
    if (!handler)
    {
        ALOGE("Unsupported SomeIP request method id 0x%04X", methodId);
        return;
    }

    std::vector<uint8_t> response_data;
    bool ret = handler(msg->getData(), msg->getLength(), response_data);
    if (!ret)
    {
        ALOGE("Process SomeIP Request fail");
        return;
    }

    std::shared_ptr<SomeipMessage> response = std::make_shared<SomeipMessage>(msg->createResponse(response_data));
    ret = someip_send_message(response);
    if (!ret)
        ALOGE("Send SomeIP Response fail");
}
