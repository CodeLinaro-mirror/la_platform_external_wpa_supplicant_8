/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "aidl_helper.h"
#include "HostapdCallback.h"

using aidl::android::hardware::wifi::hostapd::Hostapd;	
using aidl::android::hardware::wifi::hostapd::HostapdCallback;
static std::shared_ptr<Hostapd> hostapd_object;

void hostapd_init(std::shared_ptr<Hostapd> hostapd){
	hostapd_object = hostapd;
	std::shared_ptr<HostapdCallback> hostapdcallback = std::make_shared<HostapdCallback>();
	hostapd_object->registerCallback(hostapdcallback);
}

std::shared_ptr<Hostapd> getHostapdObject(){
	if(hostapd_object){
		return hostapd_object;
	}
	return nullptr;
}
