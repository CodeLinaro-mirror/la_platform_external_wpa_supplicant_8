/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "hostapd.h"

using aidl::android::hardware::wifi::hostapd::Hostapd;  

void hostapd_init(std::shared_ptr<Hostapd> hostapd);
std::shared_ptr<Hostapd> getHostapdObject();
