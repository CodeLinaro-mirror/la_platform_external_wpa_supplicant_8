/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "hostapd.h"
#include "hostapd_vendor.h"

using aidl::android::hardware::wifi::hostapd::Hostapd;  
using aidl::vendor::qti::hardware::wifi::hostapd::HostapdVendor;

void hostapd_init(std::shared_ptr<Hostapd> hostapd);
void hostapd_vendor_init(std::shared_ptr<HostapdVendor> hostapd_vendor);

std::shared_ptr<Hostapd> getHostapdObject();
std::shared_ptr<HostapdVendor> getHostapdVendorObject();
