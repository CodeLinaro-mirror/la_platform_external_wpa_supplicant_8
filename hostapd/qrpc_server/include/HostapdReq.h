/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

bool HostapdMsgHandlerAddAccessPoint(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool HostapdMsgHandlerForceClientDisconnect(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool HostapdMsgHandlerRemoveAccessPoint(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool HostapdMsgHandlerSetDebugParams(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
bool HostapdMsgHandlerTerminate(uint8_t* data, size_t length, std::vector<uint8_t>& outData);
