/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "aidl_helper.h"
#include "HostapdCallback.h"
#include "HostapdVendorCallback.h"
	
using aidl::android::hardware::wifi::hostapd::HostapdCallback;
using aidl::vendor::qti::hardware::wifi::hostapd::HostapdVendorCallback;

static std::shared_ptr<Hostapd> hostapd_object;
static std::shared_ptr<HostapdVendor> hostapd_vendor_object;

void hostapd_init(std::shared_ptr<Hostapd> hostapd){
	hostapd_object = hostapd;
	std::shared_ptr<HostapdCallback> hostapdcallback = std::make_shared<HostapdCallback>();
	hostapd_object->registerCallback(hostapdcallback);
}

void hostapd_vendor_init(std::shared_ptr<HostapdVendor> hostapd_vendor){
	hostapd_vendor_object = hostapd_vendor;
	std::shared_ptr<HostapdVendorCallback> hostapdvendorcallback = std::make_shared<HostapdVendorCallback>();
	hostapd_vendor_object->registerHostapdVendorCallback(hostapdvendorcallback);
}

std::shared_ptr<Hostapd> getHostapdObject(){
	if(hostapd_object){
		return hostapd_object;
	}
	return nullptr;
}

std::shared_ptr<HostapdVendor> getHostapdVendorObject(){
	if(hostapd_vendor_object){
		return hostapd_vendor_object;
	}
	return nullptr;
}

