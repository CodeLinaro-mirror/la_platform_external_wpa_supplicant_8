/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

bool SupplicantMsgHandlerAddStaInterface(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerGetDebugLevel(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerIsDebugShowKeysEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerIsDebugShowTimestampEnabled(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerListInterfaces(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerRemoveInterface(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerSetConcurrencyPriority(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerSetDebugParams(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool SupplicantMsgHandlerTerminate(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
