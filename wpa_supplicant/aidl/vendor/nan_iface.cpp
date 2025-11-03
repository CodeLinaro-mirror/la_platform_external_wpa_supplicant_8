/*
 * WPA Supplicant - Nan Iface Aidl interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "nan_iface.h"
#include "aidl_return_util.h"
#include "aidl_manager.h"
#include "misc_utils.h"

using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;
using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;

NanIface::NanIface(struct wpa_global* global, const std::string& ifname)
	: wpa_global_(global), ifname_(ifname), is_valid_(true)
{
}

void NanIface::invalidate() { is_valid_ = false; }
bool NanIface::isValid()
{
	return (
		is_valid_ && (wpa_supplicant_get_iface(
				  wpa_global_, ifname_.c_str()) != nullptr));
}

::ndk::ScopedAStatus NanIface::registerEventCallback(
	const std::shared_ptr<ISupplicantNanIfaceEventCallback>& in_callback)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::registerEventCallbackInternal, in_callback);
}

::ndk::ScopedAStatus NanIface::getCapabilitiesRequest(char16_t in_cmdId)
{
	return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::enableRequest(
	char16_t in_cmdId, const NanEnableRequest& in_msg1,
	const NanConfigRequest& in_msg2)
{
	// TODO(b/457258887): call wpas_nan_start;
	return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::configRequest(
	char16_t in_cmdId, const NanConfigRequest& in_request)
{
	// TODO(b/457258887): wpa_nan_set & wpas_nan_update_conf
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::disableRequest(char16_t in_cmdId)
{
	// TODO(b/457258887): wpas_nan_stop(nan);
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::createDataInterfaceRequest(
	char16_t in_cmdId, const std::string& in_ifaceName)
{
	// TODO(b/457258887): wpa_drv_if_add
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::deleteDataInterfaceRequest(
	char16_t in_cmdId, const std::string& in_ifaceName)
{
	// TODO(b/457258887): wpa_drv_if_remove
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::getName(std::string* _aidl_return)
{
	*_aidl_return = ifname_;
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::startPublishRequest(
	char16_t in_cmdId, const NanPublishRequest& in_msg)
{
	// TODO(b/457258887): wpas_nan_publish
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::stopPublishRequest(
	char16_t in_cmdId, int8_t in_sessionId)
{
	// TODO(b/457258887): wpas_nan_cancel_publish
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::startSubscribeRequest(
	char16_t in_cmdId, const NanSubscribeRequest& in_msg)
{
	// TODO(b/457258887): wpas_nan_subscribe
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::stopSubscribeRequest(
	char16_t in_cmdId, int8_t in_sessionId)
{
	// TODO(b/457258887): wpas_nan_cancel_subscribe
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::transmitFollowupRequest(
	char16_t in_cmdId, const NanTransmitFollowupRequest& in_msg)
{
	// TODO(b/457258887): wpas_nan_transmit
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::registerEventCallbackInternal(
	const std::shared_ptr<ISupplicantNanIfaceEventCallback>& callback)
{
	// TODO(b/457258887): add register event callback implementation
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}
