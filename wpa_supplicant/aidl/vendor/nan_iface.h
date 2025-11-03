/*
 * WPA Supplicant - Nan Iface Aidl interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef SUPPLICANT_NAN_IFACE_H
#define SUPPLICANT_NAN_IFACE_H

#include <aidl/android/hardware/wifi/supplicant/SupplicantStatusCode.h>

#include <aidl/android/system/wifi/mainline_supplicant/BnSupplicantNanIface.h>

extern "C"
{
#include "utils/common.h"
#include "utils/includes.h"
#include "wpa_supplicant_i.h"
}

using aidl::android::system::wifi::mainline_supplicant::BnSupplicantNanIface;

class NanIface : public BnSupplicantNanIface
{
public:
	using ISupplicantNanIfaceEventCallback =
		::aidl::android::system::wifi::mainline_supplicant::ISupplicantNanIfaceEventCallback;
	using NanConfigRequest = ::aidl::android::system::wifi::mainline_supplicant::NanConfigRequest;
	using NanEnableRequest = ::aidl::android::system::wifi::mainline_supplicant::NanEnableRequest;
	using NanPublishRequest = ::aidl::android::system::wifi::mainline_supplicant::NanPublishRequest;
	using NanSubscribeRequest = ::aidl::android::system::wifi::mainline_supplicant::NanSubscribeRequest;
	using NanTransmitFollowupRequest =
		::aidl::android::system::wifi::mainline_supplicant::NanTransmitFollowupRequest;
	NanIface(struct wpa_global* global, const std::string& ifname);
	~NanIface() override = default;

	// Refer to |StaIface::invalidate()|.
	void invalidate();
	bool isValid();

	::ndk::ScopedAStatus registerEventCallback(
		const std::shared_ptr<ISupplicantNanIfaceEventCallback>&
		in_callback) override;
	::ndk::ScopedAStatus getCapabilitiesRequest(char16_t in_cmdId) override;
	::ndk::ScopedAStatus configRequest(
		char16_t in_cmdId, const NanConfigRequest& in_request) override;
	::ndk::ScopedAStatus enableRequest(
		char16_t in_cmdId, const NanEnableRequest& in_msg1,
		const NanConfigRequest& in_msg2) override;
	::ndk::ScopedAStatus disableRequest(char16_t in_cmdId) override;
	::ndk::ScopedAStatus createDataInterfaceRequest(
		char16_t, const std::string& in_ifaceName) override;
	::ndk::ScopedAStatus deleteDataInterfaceRequest(
		char16_t, const std::string& in_ifaceName) override;
	::ndk::ScopedAStatus getName(std::string* _aidl_return) override;
	::ndk::ScopedAStatus startPublishRequest(
		char16_t in_cmdId, const NanPublishRequest& in_msg) override;
	::ndk::ScopedAStatus stopPublishRequest(
		char16_t in_cmdId, int8_t in_sessionId) override;
	::ndk::ScopedAStatus startSubscribeRequest(
		char16_t in_cmdId, const NanSubscribeRequest& in_msg) override;
	::ndk::ScopedAStatus stopSubscribeRequest(
		char16_t in_cmdId, int8_t in_sessionId) override;
	::ndk::ScopedAStatus transmitFollowupRequest(
		char16_t in_cmdId,
		const NanTransmitFollowupRequest& in_msg) override;

private:
	::ndk::ScopedAStatus registerEventCallbackInternal(
		const std::shared_ptr<ISupplicantNanIfaceEventCallback>&
		in_callback);
	struct wpa_global* wpa_global_;
	// Name of the iface this aidl object controls
	const std::string ifname_;
	bool is_valid_;
};

#endif	// SUPPLICANT_NAN_IFACE_H
