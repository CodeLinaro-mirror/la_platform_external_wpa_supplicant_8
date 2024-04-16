/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

bool StaIfaceMsgHandlerAddNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerListNetworks(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerReassociate(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetPowerSave(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetMacAddress(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStopRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandleAddRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetBtCoexistenceMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetBtCoexistenceScanModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetSuspendModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetCountryCode(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetExternalSim(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerEnableAutoReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetKeyMgmtCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetConnectionCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetWpaDriverCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetConnectionMloLinksInfo(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetSignalPollResults(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
