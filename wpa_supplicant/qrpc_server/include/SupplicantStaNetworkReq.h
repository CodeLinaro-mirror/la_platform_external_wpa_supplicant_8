/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

bool StaNetworkMsgHandlerGetId(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetWepTxKeyIdx(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetBssid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetScanSsid(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetKeyMgmt(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetProto(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetAuthAlg(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetGroupCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetPairwiseCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetPskPassphrase(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetPsk(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetWepKey(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetWepTxKeyIdx(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetRequirePmf(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetIdStr(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerEnable(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerDisable(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSelect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerGetGroupMgmtCipher(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetSaePassword(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetPmkCache(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaNetworkMsgHandlerSetSaeH2eMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
