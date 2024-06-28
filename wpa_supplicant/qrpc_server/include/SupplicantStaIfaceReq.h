/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

bool StaIfaceMsgHandlerAddDppPeerUri(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerAddExtRadioWork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerAddNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerAddRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerCancelWps(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerEnableAutoReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerFilsHlpAddRequest(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerFilsHlpFlushRequest(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGenerateDppBootstrapInfoForResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGenerateSelfDppConfiguration(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetConnectionCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetConnectionMloLinksInfo(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetKeyMgmtCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetMacAddress(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetName(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetType(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetWpaDriverCapabilities(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateAnqpQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateHs20IconQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateTdlsDiscover(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateTdlsSetup(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateTdlsTeardown(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerInitiateVenueUrlAnqpQuery(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerListNetworks(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerReassociate(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerReconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetQosPolicyFeatureEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSendQosPolicyResponse(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveAllQosPolicies(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveDppUri(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveExtRadioWork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveNetwork(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetBtCoexistenceMode(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetBtCoexistenceScanModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetCountryCode(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetExternalSim(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetMboCellularDataStatus(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetPowerSave(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetSuspendModeEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsConfigMethods(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsDeviceName(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsDeviceType(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsManufacturer(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsModelName(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsModelNumber(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerSetWpsSerialNumber(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartDppConfiguratorInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartDppEnrolleeInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartDppEnrolleeResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartWpsPbc(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartWpsPinDisplay(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartWpsPinKeypad(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStartWpsRegistrar(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStopDppInitiator(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStopDppResponder(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerStopRxFilter(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerGetSignalPollResults(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerAddQosPolicyRequestForScs(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool StaIfaceMsgHandlerRemoveQosPolicyForScs(uint8_t* data, size_t length, std::vector<uint8_t>& outData);