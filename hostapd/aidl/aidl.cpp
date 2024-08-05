/*
 * aidl interface for hostapd daemon
 * Copyright (c) 2004-2018, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2018, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */
/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "hostapd.h"
#include "HostapdCallback.h"
#include "aidl_helper.h"

#ifdef CONFIG_SOMEIP_SUPPORT
#include "hostapd_someip_server.h"
#endif /* CONFIG_SOMEIP_SUPPORT */

#ifdef CONFIG_USE_VENDOR_AIDL
#include "hostapd_vendor.h"
#endif
#ifdef CONFIG_HOSTAPD_RPC
#include <cutils/properties.h>
#include <hostapd_rpc_api.h>
#endif
//#include <android/binder_process.h>
//#include <android/binder_manager.h>
extern "C"
{
#include "aidl.h"
#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/includes.h"
#include "aidl_sock.h"
}

using aidl::android::hardware::wifi::hostapd::Hostapd;

#ifdef CONFIG_USE_VENDOR_AIDL
using aidl::vendor::qti::hardware::wifi::hostapd::HostapdVendor;
#endif

// This file is a bridge between the hostapd code written in 'C' and the aidl
// interface in C++. So, using "C" style static globals here!
static int aidl_fd = -1;
static std::shared_ptr<Hostapd> service;
#ifdef CONFIG_USE_VENDOR_AIDL
static std::shared_ptr<HostapdVendor> service_vendor;
#endif

#ifdef CONFIG_CTRL_IFACE_AIDL
int hostapd_aidl_init(struct hapd_interfaces *interfaces)
{
	wpa_printf(MSG_INFO, "Initializing aidl control");
	hostapd_create_aidl_socket(&aidl_fd);
	if (aidl_fd < 0)
		goto err;
	wpa_printf(MSG_INFO, "Processing aidl events on FD %d", aidl_fd);
	// Look for read events from the aidl socket in the eloop.
	if (eloop_register_read_sock(
		aidl_fd, hostapd_aidl_sock_handler, interfaces, NULL) < 0)
		goto err;

#ifdef CONFIG_HOSTAPD_RPC
	if (property_get_bool("persist.vendor.wlan.hal.rpc", false)) {
		if (!HostapdRpcStartClient()) {
			wpa_printf(MSG_ERROR, "hostapd rpc client fail");
			goto err;
		}

		return 0;
	}
#endif
	wpa_printf(MSG_DEBUG, "Make service");
	service = std::make_shared<Hostapd>(interfaces);
	if (!service)
		goto err;
	hostapd_init(service);
#ifdef CONFIG_SOMEIP_SUPPORT
	if (!HostapdSomeIPServerInit()){
		wpa_printf(MSG_ERROR, "Failed to initialize hostapd someip server");		
		goto err;
	}
	if (!HostapdSomeIPServerStart()){
		wpa_printf(MSG_ERROR, "Failed to start hostapd someip server");	 
		goto err;
	}
#endif

	
#ifdef CONFIG_USE_VENDOR_AIDL
	service_vendor = std::make_shared<HostapdVendor>(interfaces);
	if (!service_vendor)
		goto err;
#endif
	return 0;
err:

#ifdef CONFIG_SOMEIP_SUPPORT
	HostapdSomeIPServerDeinit();
#endif
	hostapd_aidl_deinit(interfaces);
	return -1;
}

void hostapd_aidl_deinit(struct hapd_interfaces *interfaces)
{
	wpa_printf(MSG_INFO, "Deiniting aidl control");
#ifdef CONFIG_HOSTAPD_RPC
	if (property_get_bool("persist.vendor.wlan.hal.rpc", false)) {
		HostapdRpcStopClient();
	} else
#endif
	// Before aidl deinit, make sure call terminate to clear callback_
	eloop_unregister_read_sock(aidl_fd);
	hostapd_destroy_aidl_socket();
	if (service) {
		service->terminate();
	}
	
}
#endif
