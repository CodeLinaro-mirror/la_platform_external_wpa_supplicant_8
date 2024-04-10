/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <map>
#include <functional>

#include <rpc/util/common_util.h>
#include <rpc/util/someip_api.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>

#include "supplicant_message_handler.h"

#include "SupplicantReq.h"
#include "SupplicantStaIfaceReq.h"
#include "SupplicantStaNetworkReq.h"

typedef std::function< bool (uint8_t* data, size_t length, std::vector<uint8_t>& outData) > MessageHandler;
static std::map<uint16_t, MessageHandler> msgHandlerMap = {
    /* ISupplicant method handlers */
    {SUPPLICANT_ADD_STA_INTERFACE_REQ, &SupplicantMsgHandlerAddStaInterface},
    {SUPPLICANT_REMOVE_INTERFACE_REQ, &SupplicantMsgHandlerRemoveInterface},
    {SUPPLICANT_SET_DEBUG_PARAMS_REQ, &SupplicantMsgHandlerSetDebugParams},
    {SUPPLICANT_SET_CONCURRENCY_PRIORITY_REQ, &SupplicantMsgHandlerSetConcurrencyPriority},
    {SUPPLICANT_TERMINATE_REQ, &SupplicantMsgHandlerTerminate},

    /* ISupplicantStaIface method handlers */
    {SUPPLICANT_STA_IFACE_ADD_NETWORK_REQ, &StaIfaceMsgHandlerAddNetwork},
    {SUPPLICANT_STA_IFACE_REMOVE_NETWORK_REQ, &StaIfaceMsgHandlerRemoveNetwork},
    {SUPPLICANT_STA_IFACE_LIST_NETWORKS_REQ, &StaIfaceMsgHandlerListNetworks},
    {SUPPLICANT_STA_IFACE_REASSOCIATE_REQ, &StaIfaceMsgHandlerReassociate},
    {SUPPLICANT_STA_IFACE_RECONNECT_REQ, &StaIfaceMsgHandlerReconnect},
    {SUPPLICANT_STA_IFACE_DISCONNECT_REQ, &StaIfaceMsgHandlerDisconnect},
    {SUPPLICANT_STA_IFACE_SET_POWER_SAVE_REQ, &StaIfaceMsgHandlerSetPowerSave},
    {SUPPLICANT_STA_IFACE_GET_MAC_ADDRESS_REQ, &StaIfaceMsgHandlerGetMacAddress},
    {SUPPLICANT_STA_IFACE_START_RX_FILTER_REQ, &StaIfaceMsgHandlerStartRxFilter},
    {SUPPLICANT_STA_IFACE_STOP_RX_FILTER_REQ, &StaIfaceMsgHandlerStopRxFilter},
    {SUPPLICANT_STA_IFACE_ADD_RX_FILTER_REQ, &StaIfaceMsgHandleAddRxFilter},
    {SUPPLICANT_STA_IFACE_REMOVE_RX_FILTER_REQ, &StaIfaceMsgHandlerRemoveRxFilter},
    {SUPPLICANT_STA_IFACE_SET_BT_COEXISTENCE_MODE_REQ, &StaIfaceMsgHandlerSetBtCoexistenceMode},
    {SUPPLICANT_STA_IFACE_SET_BT_COEXISTENCE_SCAN_MODE_ENABLED_REQ, &StaIfaceMsgHandlerSetBtCoexistenceScanModeEnabled},
    {SUPPLICANT_STA_IFACE_SET_SUSPEND_MODE_ENABLED_REQ, &StaIfaceMsgHandlerSetSuspendModeEnabled},
    {SUPPLICANT_STA_IFACE_SET_COUNTRY_CODE_REQ, &StaIfaceMsgHandlerSetCountryCode},
    {SUPPLICANT_STA_IFACE_SET_EXTERNAL_SIM_REQ, &StaIfaceMsgHandlerSetExternalSim},
    {SUPPLICANT_STA_IFACE_ENABLE_AUTO_RECONNECT_REQ, &StaIfaceMsgHandlerEnableAutoReconnect},
    {SUPPLICANT_STA_IFACE_GET_KEY_MGMT_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetKeyMgmtCapabilities},
    {SUPPLICANT_STA_IFACE_GET_CONNECTION_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetConnectionCapabilities},
    {SUPPLICANT_STA_IFACE_GET_WPA_DRIVER_CAPABILITIES_REQ, &StaIfaceMsgHandlerGetWpaDriverCapabilities},
    {SUPPLICANT_STA_IFACE_GET_SIGNAL_POLL_RESULTS_REQ, &StaIfaceMsgHandlerGetSignalPollResults},

    /* ISupplicantStaNetwork method handlers */
    {SUPPLICANT_STA_NETWORK_GET_ID_REQ, &StaNetworkMsgHandlerGetId},
    {SUPPLICANT_STA_NETWORK_SET_SSID_REQ, &StaNetworkMsgHandlerSetSsid},
    {SUPPLICANT_STA_NETWORK_SET_BSSID_REQ, &StaNetworkMsgHandlerSetBssid},
    {SUPPLICANT_STA_NETWORK_SET_SCAN_SSID_REQ, &StaNetworkMsgHandlerSetScanSsid},
    {SUPPLICANT_STA_NETWORK_SET_KEY_MGMT_REQ, &StaNetworkMsgHandlerSetKeyMgmt},
    {SUPPLICANT_STA_NETWORK_SET_PROTO_REQ, &StaNetworkMsgHandlerSetProto},
    {SUPPLICANT_STA_NETWORK_SET_AUTH_ALG_REQ, &StaNetworkMsgHandlerSetAuthAlg},
    {SUPPLICANT_STA_NETWORK_SET_GROUP_CIPHER_REQ, &StaNetworkMsgHandlerSetGroupCipher},
    {SUPPLICANT_STA_NETWORK_SET_PAIRWISE_CIPHER_REQ, &StaNetworkMsgHandlerSetPairwiseCipher},
    {SUPPLICANT_STA_NETWORK_SET_PSK_PASSPHRASE_REQ, &StaNetworkMsgHandlerSetPskPassphrase},
    {SUPPLICANT_STA_NETWORK_SET_PSK_REQ, &StaNetworkMsgHandlerSetPsk},
    {SUPPLICANT_STA_NETWORK_SET_WEP_KEY_REQ, &StaNetworkMsgHandlerSetWepKey},
    {SUPPLICANT_STA_NETWORK_SET_WEP_TX_KEY_IDX_REQ, &StaNetworkMsgHandlerSetWepTxKeyIdx},
    {SUPPLICANT_STA_NETWORK_SET_REQUIRE_PMF_REQ, &StaNetworkMsgHandlerSetRequirePmf},
    {SUPPLICANT_STA_NETWORK_SET_ID_STR_REQ, &StaNetworkMsgHandlerSetIdStr},
    {SUPPLICANT_STA_NETWORK_GET_SSID_REQ, &StaNetworkMsgHandlerGetSsid},
    {SUPPLICANT_STA_NETWORK_GET_BSSID_REQ, &StaNetworkMsgHandlerGetBssid},
    {SUPPLICANT_STA_NETWORK_GET_SCAN_SSID_REQ, &StaNetworkMsgHandlerGetScanSsid},
    {SUPPLICANT_STA_NETWORK_GET_KEY_MGMT_REQ, &StaNetworkMsgHandlerGetKeyMgmt},
    {SUPPLICANT_STA_NETWORK_GET_PROTO_REQ, &StaNetworkMsgHandlerGetProto},
    {SUPPLICANT_STA_NETWORK_GET_AUTH_ALG_REQ, &StaNetworkMsgHandlerGetAuthAlg},
    {SUPPLICANT_STA_NETWORK_GET_GROUP_CIPHER_REQ, &StaNetworkMsgHandlerGetGroupCipher},
    {SUPPLICANT_STA_NETWORK_GET_PAIRWISE_CIPHER_REQ, &StaNetworkMsgHandlerGetPairwiseCipher},
    {SUPPLICANT_STA_NETWORK_GET_PSK_PASSPHRASE_REQ, &StaNetworkMsgHandlerGetPskPassphrase},
    {SUPPLICANT_STA_NETWORK_GET_PSK_REQ, &StaNetworkMsgHandlerGetPsk},
    {SUPPLICANT_STA_NETWORK_GET_SAE_PASSWORD_REQ, &StaNetworkMsgHandlerGetSaePassword},
    {SUPPLICANT_STA_NETWORK_GET_WEP_KEY_REQ, &StaNetworkMsgHandlerGetWepKey},
    {SUPPLICANT_STA_NETWORK_GET_WEP_TX_KEY_IDX_REQ, &StaNetworkMsgHandlerGetWepTxKeyIdx},
    {SUPPLICANT_STA_NETWORK_GET_REQUIRE_PMF_REQ, &StaNetworkMsgHandlerGetRequirePmf},
    {SUPPLICANT_STA_NETWORK_GET_ID_STR_REQ, &StaNetworkMsgHandlerGetIdStr},
    {SUPPLICANT_STA_NETWORK_ENABLE_REQ, &StaNetworkMsgHandlerEnable},
    {SUPPLICANT_STA_NETWORK_DISABLE_REQ, &StaNetworkMsgHandlerDisable},
    {SUPPLICANT_STA_NETWORK_SELECT_REQ, &StaNetworkMsgHandlerSelect},
    {SUPPLICANT_STA_NETWORK_SET_GROUP_MGMT_CIPHER_REQ, &StaNetworkMsgHandlerSetGroupMgmtCipher},
    {SUPPLICANT_STA_NETWORK_GET_GROUP_MGMT_CIPHER_REQ, &StaNetworkMsgHandlerGetGroupMgmtCipher},
    {SUPPLICANT_STA_NETWORK_SET_SAE_PASSWORD_REQ, &StaNetworkMsgHandlerSetSaePassword},
    {SUPPLICANT_STA_NETWORK_SET_PMK_CACHE_REQ, &StaNetworkMsgHandlerSetPmkCache},
    {SUPPLICANT_STA_NETWORK_SET_SAE_H2E_MODE_REQ, &StaNetworkMsgHandlerSetSaeH2eMode}
};

static inline MessageHandler SupplicantGetMessageHandler(uint16_t methodId)
{
    auto item = msgHandlerMap.find(methodId);
    if (item != msgHandlerMap.end())
        return item->second;

    return nullptr;
}

void SupplicantProcessSomeIPRequestMessage(uint16_t methodId, uint8_t *data, size_t length)
{
    ALOGI("Recv Someip Request message with method_id 0x%04X", methodId);

    MessageHandler handler = SupplicantGetMessageHandler(methodId);
    if (!handler) {
        ALOGE("Unspported SomeIP request method id 0x%04X", methodId);
        return;
    }

    std::vector<uint8_t> response;
    bool ret = handler(data, length, response);
    if (!ret) {
        ALOGE("Process SomeIP Request fail");
        return;
    }

    ret = someip_send_response(methodId, response.data(), response.size());
    if (!ret)
        ALOGE("Send SomeIP Response fail");
}
