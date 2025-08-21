/*
 * aidl interface for wpa_supplicant daemon
 * Copyright (c) 2004-2018, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2018, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "hostapd.h"
#ifdef CONFIG_USE_VENDOR_AIDL
#include "hostapd_vendor.h"
#endif
#include <android/binder_process.h>
#include <android/binder_manager.h>

extern "C"
{
#include "aidl.h"
#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/includes.h"
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

void hostapd_aidl_sock_handler(
    int /* sock */, void * /* eloop_ctx */, void * /* sock_ctx */)
{
	ABinderProcess_handlePolledCommands();
}

int hostapd_aidl_init(struct hapd_interfaces *interfaces)
{
	wpa_printf(MSG_INFO, "Initializing aidl control");
	wpa_printf(MSG_INFO, "Interface version: %d", Hostapd::version);
	std::string instance;   // declared here to allow use of goto
#ifdef CONFIG_USE_VENDOR_AIDL
	std::string instance_vendor;
#endif

	ABinderProcess_setupPolling(&aidl_fd);
	if (aidl_fd < 0)
		goto err;

	wpa_printf(MSG_INFO, "Processing aidl events on FD %d", aidl_fd);
	// Look for read events from the aidl socket in the eloop.
	if (eloop_register_read_sock(
		aidl_fd, hostapd_aidl_sock_handler, interfaces, NULL) < 0)
		goto err;

	wpa_printf(MSG_DEBUG, "Make service");
	service = ndk::SharedRefBase::make<Hostapd>(interfaces);
	if (!service)
		goto err;
	wpa_printf(MSG_DEBUG, "Add service");
	instance = std::string() + Hostapd::descriptor + "/default";
	if (AServiceManager_addService(service->asBinder().get(), instance.c_str()) != STATUS_OK)
		goto err;
#ifdef CONFIG_USE_VENDOR_AIDL
	service_vendor = ndk::SharedRefBase::make<HostapdVendor>(interfaces);
	if (!service_vendor)
		goto err;
	instance_vendor = std::string() + HostapdVendor::descriptor + "/default";
	if (AServiceManager_addService(service_vendor->asBinder().get(), instance_vendor.c_str()) != STATUS_OK)
		goto err;
	wpa_printf(MSG_INFO, "Add vendor service %s", instance_vendor.c_str());
#endif
	return 0;
err:
	hostapd_aidl_deinit(interfaces);
	return -1;
}

void hostapd_aidl_deinit(struct hapd_interfaces *interfaces)
{
	wpa_printf(MSG_INFO, "Deiniting aidl control");
	// Before aidl deinit, make sure call terminate to clear callback_
	if (service) {
		service->terminate();
	}
	eloop_unregister_read_sock(aidl_fd);
	aidl_fd = -1;
}
