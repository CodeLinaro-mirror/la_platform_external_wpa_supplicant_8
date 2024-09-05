/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_SOMEIP_COMMON_H
#define SUPPLICANT_SOMEIP_COMMON_H

#define SUPPLICANT_PAYLOAD_MIN_SIZE 4

#include <cstdint>
#include <vector>

bool SupplicantParseInstanceId(uint8_t* data, size_t length, uint16_t& staIfaceId, uint16_t& networkId);
void SupplicantAddInstanceId(std::vector<uint8_t>& data, uint16_t staIfaceId = 0, uint16_t networkId = 0);

#endif /* SUPPLICANT_SOMEIP_COMMON_H */