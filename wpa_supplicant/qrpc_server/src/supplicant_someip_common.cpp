/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "supplicant_someip_common.h"

bool SupplicantParseInstanceId(uint8_t* data, size_t length, uint16_t& staIfaceId, uint16_t& networkId)
{
    if (!data || length < SUPPLICANT_PAYLOAD_MIN_SIZE) {
        return false;
    }

    staIfaceId = (uint16_t)data[0] | (((uint16_t)data[1]) << 8);
    networkId = (uint16_t)data[2] | (((uint16_t)data[3]) << 8);

    return true;
}

void SupplicantAddInstanceId(std::vector<uint8_t>& data, uint16_t staIfaceId, uint16_t networkId)
{
    data.insert(data.begin(), (uint8_t)((networkId >> 8) & 0xff));
    data.insert(data.begin(), (uint8_t)(networkId & 0xff));
    data.insert(data.begin(), (uint8_t)((staIfaceId >> 8) & 0xff));
    data.insert(data.begin(), (uint8_t)(staIfaceId & 0xff));
}
