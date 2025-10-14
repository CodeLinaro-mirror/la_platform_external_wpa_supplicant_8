/*
 * WPA Supplicant - Nan Iface Aidl interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "nan_iface.h"
#include <thread>
#include "aidl_return_util.h"
#include "aidl_manager.h"
#include "misc_utils.h"
#include "driver_i.h"

using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;
using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;
using NanStatusCode = aidl::android::system::wifi::mainline_supplicant::NanStatus::NanStatusCode;

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
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::getCapabilitiesRequestInternal, in_cmdId);
}

::ndk::ScopedAStatus NanIface::enableRequest(
	char16_t in_cmdId, const NanEnableRequest& in_msg1,
	const NanConfigRequest& in_msg2)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::enableRequestInternal, in_cmdId, in_msg1, in_msg2);
}

::ndk::ScopedAStatus NanIface::configRequest(
	char16_t in_cmdId, const NanConfigRequest& in_request)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::configRequestInternal, in_cmdId, in_request);
}

::ndk::ScopedAStatus NanIface::disableRequest(char16_t in_cmdId)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::disableRequestInternal, in_cmdId);
}

::ndk::ScopedAStatus NanIface::createDataInterfaceRequest(
	char16_t in_cmdId, const std::string& in_ifaceName)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::createDataInterfaceRequestInternal, in_cmdId, in_ifaceName
	);
}

::ndk::ScopedAStatus NanIface::deleteDataInterfaceRequest(
	char16_t in_cmdId, const std::string& in_ifaceName)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::deleteDataInterfaceRequestInternal, in_cmdId, in_ifaceName
	);
}

::ndk::ScopedAStatus NanIface::getName(std::string* _aidl_return)
{
	*_aidl_return = ifname_;
	return  createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

::ndk::ScopedAStatus NanIface::startPublishRequest(
	char16_t in_cmdId, const NanPublishRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::startPublishRequestInternal, in_cmdId, in_msg
	);
}

::ndk::ScopedAStatus NanIface::stopPublishRequest(
	char16_t in_cmdId, int8_t in_sessionId)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::stopPublishRequestInternal, in_cmdId, in_sessionId
	);
}

::ndk::ScopedAStatus NanIface::startSubscribeRequest(
	char16_t in_cmdId, const NanSubscribeRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::startSubscribeRequestInternal, in_cmdId, in_msg
	);
}

::ndk::ScopedAStatus NanIface::stopSubscribeRequest(
	char16_t in_cmdId, int8_t in_sessionId)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::stopSubscribeRequestInternal, in_cmdId, in_sessionId
	);
}

::ndk::ScopedAStatus NanIface::transmitFollowupRequest(
	char16_t in_cmdId, const NanTransmitFollowupRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::transmitFollowupRequestInternal, in_cmdId, in_msg
	);
}

::ndk::ScopedAStatus NanIface::registerEventCallbackInternal(
	const std::shared_ptr<ISupplicantNanIfaceEventCallback>& callback)
{
    AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager ||
		!aidl_manager->addNanIfaceCallbackAidlObject(ifname_, callback)) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::getCapabilitiesRequestInternal(char16_t cmdId)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	NanCapabilities aidl_caps = {};
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: get capabilities from |wpa_supplicant| and |struct wpa_sdriver_capa|
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanCapabilitiesResponse(ifname, cmdId, nan_status, aidl_caps);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::configRequestInternal(
	char16_t cmdId, const NanConfigRequest& request)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_set() and wpas_nan_update_conf()
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanConfigResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::enableRequestInternal(
	char16_t cmdId, const NanEnableRequest& msg1,
	const NanConfigRequest& msg2)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_set() & wpas_nan_start()
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanEnableResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::disableRequestInternal(char16_t cmdId)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	// TODO: wpas_nan_stop(nan);
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanDisableResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::createDataInterfaceRequestInternal(
	char16_t cmdId, const std::string& ifaceName)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpa_drv_if_add & wpas_supplicant_add_iface
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::deleteDataInterfaceRequestInternal(
	char16_t cmdId, const std::string& ifaceName)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpa_drv_if_remove & wpas_supplicant_remove_iface
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanDeleteDataInterfaceResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::startPublishRequestInternal(
	char16_t cmdId, const NanPublishRequest& msg)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_publish
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanStartPublishResponse(ifname, cmdId, nan_status, 0);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::stopPublishRequestInternal(
	char16_t cmdId, int8_t sessionId)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_cancel_publish
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanStopPublishResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::startSubscribeRequestInternal(
	char16_t cmdId, const NanSubscribeRequest& msg)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_subscribe
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanStartSubscribeResponse(ifname, cmdId, nan_status, 0);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::stopSubscribeRequestInternal(
	char16_t cmdId, int8_t sessionId)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_cancel_subscribe
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanStopSubscribeResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::transmitFollowupRequestInternal(
	char16_t cmdId, const NanTransmitFollowupRequest& msg)
{
	NanStatus nan_status;
	nan_status.status = NanStatusCode::SUCCESS;
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	// TODO: wpas_nan_transmit
	std::thread([=, ifname = ifname_] {
		aidl_manager->notifyNanTransmitFollowupResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}
