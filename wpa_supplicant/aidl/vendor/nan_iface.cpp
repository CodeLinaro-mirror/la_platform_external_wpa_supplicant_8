/*
 * WPA Supplicant - Nan Iface Aidl interface
 * Copyright (c) 2025, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "aidl_manager.h"
#include "aidl_return_util.h"
#include "aidl/shared/shared_utils.h"
#include "driver_i.h"
#include "misc_utils.h"
#include "nan_iface.h"
#include "nan_supplicant.h"
#include "nan_utils.h"
#include "src/common/nan_de.h"
#include "src/common/nan_defs.h"

#include <iomanip>
#include <sstream>
#include <thread>

using aidl::android::hardware::wifi::supplicant::AidlManager;
using aidl::android::hardware::wifi::supplicant::aidl_return_util::validateAndCall;
using aidl::android::hardware::wifi::supplicant::misc_utils::convertVectorToWpaBuf;
using aidl::android::hardware::wifi::supplicant::misc_utils::createStatus;
using aidl::android::system::wifi::mainline_supplicant::nan_utils::validateNanPublishConfig;
using aidl::android::system::wifi::mainline_supplicant::nan_utils::validateNanSubscribeConfig;
using aidl::android::system::wifi::mainline_supplicant::nan_utils::convertAidlNanPublishConfigToInternal;
using aidl::android::system::wifi::mainline_supplicant::nan_utils::convertAidlNanSubscribeConfigToInternal;
using NanStatusCode = aidl::android::system::wifi::mainline_supplicant::NanStatus::NanStatusCode;

const std::string kMainlineSupplicantConfigPath =
	"/apex/com.android.wifi/etc/wpa_supplicant_mainline.conf";
static constexpr int kNanIfaceCapMaxPublishSessions = 10;
static constexpr int kNanIfaceCapMaxSubscribeSessions = kNanIfaceCapMaxPublishSessions;
static constexpr int kNanMaxServiceNameLen = 255;
static constexpr int kNanMaxServiceSpecificInfoLen = 255;
static constexpr int kNanMaxMatchFilterLen = 255;
static constexpr int kNanMaxExtendedServiceSpecificInfoLen = 1280;
static constexpr bool kNanIfaceCapInstantCommunicationModeSupport = false;
static constexpr bool kNanIfaceCapSupportsPeriodicRanging = false;
static constexpr bool kNanIfaceCapSupportsSuspension = false;
static constexpr int kNanIfaceConfBufSize = 128;
static constexpr int kNanIfaceConfBandRssiClose = -60;
static constexpr int kNanIfaceConfBandRssiMiddle = -75;
static constexpr int kNanIfaceConfBandDisableScan = 0;
static constexpr int kNanIfaceConfScanDwellTime = 150;
static constexpr int kNanIfaceConfScanPeriod = 20;

template <typename... Args>
int setNanConfigParam(struct wpa_supplicant* wpa_s, const char* param, Args... args)
{
	char cmd[kNanIfaceConfBufSize];
	int ret = snprintf(cmd, kNanIfaceConfBufSize, param, args...);
	if (ret < 0 || ret >= sizeof(cmd)) {
		return 1;
	}
	return wpas_nan_set(wpa_s, cmd);
}

NanIface::NanIface(struct wpa_global* global, const std::string& ifname)
	: wpa_global_(global), ifname_(ifname), is_valid_(true), started_cluster_indication_(false),
	  joined_cluster_indication_(false)
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

::ndk::ScopedAStatus NanIface::initiateBootstrappingRequest(
	char16_t in_cmdId, const NanBootstrappingRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::initiateBootstrappingRequestInternal, in_cmdId, in_msg);
}

::ndk::ScopedAStatus NanIface::respondToBootstrappingIndicationRequest(
	char16_t in_cmdId, const NanBootstrappingResponse& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::respondToBootstrappingIndicationRequestInternal, in_cmdId,
		in_msg);
}

::ndk::ScopedAStatus NanIface::initiatePairingRequest(
	char16_t in_cmdId, const NanPairingRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::initiatePairingRequestInternal, in_cmdId, in_msg);
}

::ndk::ScopedAStatus NanIface::respondToPairingIndicationRequest(
	char16_t in_cmdId, const NanRespondToPairingIndicationRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::respondToPairingIndicationRequestInternal, in_cmdId,
		in_msg);
}

::ndk::ScopedAStatus NanIface::terminatePairingRequest(
	char16_t in_cmdId, int32_t in_pairingInstanceId)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::terminatePairingRequestInternal, in_cmdId,
		in_pairingInstanceId);
}

::ndk::ScopedAStatus NanIface::initiateDataPathRequest(
	char16_t in_cmdId, const NanInitiateDataPathRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::initiateDataPathRequestInternal, in_cmdId, in_msg);
}

::ndk::ScopedAStatus NanIface::respondToDataPathIndicationRequest(
	char16_t in_cmdId, const NanRespondToDataPathIndicationRequest& in_msg)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::respondToDataPathIndicationRequestInternal, in_cmdId,
		in_msg);
}

::ndk::ScopedAStatus NanIface::terminateDataPathRequest(
	char16_t in_cmdId, int32_t in_ndpInstanceId)
{
	return validateAndCall(
		this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
		&NanIface::terminateDataPathRequestInternal, in_cmdId,
		in_ndpInstanceId);
}

::ndk::ScopedAStatus NanIface::registerEventCallbackInternal(
	const std::shared_ptr<ISupplicantNanIfaceEventCallback>& callback)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager ||
		aidl_manager->addNanIfaceCallbackAidlObject(ifname_, callback)) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::getCapabilitiesRequestInternal(char16_t cmdId)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::SUCCESS;
		NanCapabilities aidl_caps = {};

		aidl_caps.maxPublishes = kNanIfaceCapMaxPublishSessions;
		aidl_caps.maxSubscribes = kNanIfaceCapMaxSubscribeSessions;
		aidl_caps.maxServiceNameLen = kNanMaxServiceNameLen;
		aidl_caps.maxServiceSpecificInfoLen = kNanMaxServiceSpecificInfoLen;
		aidl_caps.maxMatchFilterLen = kNanMaxMatchFilterLen;
		aidl_caps.maxExtendedServiceSpecificInfoLen =
			kNanMaxExtendedServiceSpecificInfoLen;
		aidl_caps.instantCommunicationModeSupportFlag =
			kNanIfaceCapInstantCommunicationModeSupport;
		aidl_caps.supportsSuspension = kNanIfaceCapSupportsSuspension;
		aidl_caps.supportsPeriodicRanging = kNanIfaceCapSupportsPeriodicRanging;
		aidl_manager->notifyNanCapabilitiesResponse(
			ifname, cmdId, nan_status, aidl_caps);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::configRequestInternal(
	char16_t cmdId, const NanConfigRequest& request)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanConfigResponse(ifname, cmdId, nan_status);
			return;
		}

		// Write the input config to the nan_config inside wpa_s instance
		int ret = 0;
		ret |= setNanConfigParam(wpa_s, "master_pref %d", request.masterPref);
		ret |= setNanConfigParam(wpa_s, "scan_period %d", kNanIfaceConfScanPeriod);
		ret |= setNanConfigParam(wpa_s, "scan_dwell_time %d", kNanIfaceConfScanDwellTime);
		ret |= setNanConfigParam(
			wpa_s, "discovery_beacon_interval %d", request.discoveryBeaconIntervalMs);
		if (!request.bandSpecificConfig.empty()) {
			ret |= setNanConfigParam(
				wpa_s, "low_band_cfg %d,%d,%d,%d", kNanIfaceConfBandRssiClose,
				kNanIfaceConfBandRssiMiddle,
				request.bandSpecificConfig[0].validDiscoveryWindowIntervalVal
					? request.bandSpecificConfig[0].discoveryWindowIntervalVal : 1,
				kNanIfaceConfBandDisableScan);
			if (request.bandSpecificConfig.size() > 1) {
				ret |= setNanConfigParam(
					wpa_s, "high_band_cfg %d,%d,%d,%d", kNanIfaceConfBandRssiClose,
					kNanIfaceConfBandRssiMiddle,
					request.bandSpecificConfig[1].validDiscoveryWindowIntervalVal
						? request.bandSpecificConfig[1].discoveryWindowIntervalVal : 1,
					kNanIfaceConfBandDisableScan);
			}
		}

		ret |= setNanConfigParam(
			wpa_s, "cluster_id " MACSTR, MAC2STR(request.clusterId));
		if (ret != 0) {
			aidl_manager->notifyNanConfigResponse(
				ifname, cmdId, nan_status);
			return;
		}

		// Update the nan_config to the NAN discovery engine level
		ret = wpas_nan_update_conf(wpa_s);
		if (ret != 0) {
			aidl_manager->notifyNanConfigResponse(
				ifname, cmdId, nan_status);
			return;
		}
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanConfigResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::enableRequestInternal(
	char16_t cmdId, const NanEnableRequest& msg1,
	const NanConfigRequest& msg2)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanEnableResponse(ifname, cmdId, nan_status);
			return;
		}

		// Default with 2.4G band
		int8_t dual_band =
			(msg1.operateInBand[1] || msg1.operateInBand[2]) ? 1 : 0;
		int ret = 0;
		ret |= setNanConfigParam(wpa_s, "master_pref %d", msg2.masterPref);
		ret |= setNanConfigParam(wpa_s, "dual_band %d", dual_band);
		ret |= setNanConfigParam(wpa_s, "scan_period %d", kNanIfaceConfScanPeriod);
		ret |= setNanConfigParam(wpa_s, "scan_dwell_time %d", kNanIfaceConfScanDwellTime);
		ret |= setNanConfigParam(
			wpa_s, "discovery_beacon_interval %d",
			msg2.discoveryBeaconIntervalMs);
		if (!msg2.bandSpecificConfig.empty()) {
			ret |= setNanConfigParam(
				wpa_s, "low_band_cfg %d,%d,%d,%d", kNanIfaceConfBandRssiClose,
				kNanIfaceConfBandRssiMiddle,
				msg2.bandSpecificConfig[0].validDiscoveryWindowIntervalVal
				? msg2.bandSpecificConfig[0].discoveryWindowIntervalVal : 1,
				kNanIfaceConfBandDisableScan);
			if (dual_band && msg2.bandSpecificConfig.size() > 1) {
				ret |= setNanConfigParam(
					wpa_s, "high_band_cfg %d,%d,%d,%d", kNanIfaceConfBandRssiClose,
					kNanIfaceConfBandRssiMiddle,
					msg2.bandSpecificConfig[1].validDiscoveryWindowIntervalVal
					? msg2.bandSpecificConfig[1].discoveryWindowIntervalVal : 1,
					kNanIfaceConfBandDisableScan);
			}
		}
		ret |= setNanConfigParam(
			wpa_s, "cluster_id " MACSTR, MAC2STR(msg2.clusterId));

		if (ret != 0) {
			aidl_manager->notifyNanEnableResponse(
				ifname, cmdId, nan_status);
			return;
		}

		ret = wpas_nan_start(wpa_s);
		if (ret != 0) {
			aidl_manager->notifyNanEnableResponse(
				ifname, cmdId, nan_status);
			return;
		}
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanEnableResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::disableRequestInternal(char16_t cmdId)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanDisableResponse(ifname, cmdId, nan_status);
			return;
		}

		int ret = wpas_nan_stop(wpa_s);
		if (ret != 0) {
			aidl_manager->notifyNanDisableResponse(
				ifname, cmdId, nan_status);
			return;
		}
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanDisableResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::createDataInterfaceRequestInternal(
	char16_t cmdId, const std::string& ifaceName)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_]  {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		// Get the wpa supplicant of NMI
		struct wpa_supplicant *wpa_s = wpa_supplicant_get_iface(wpa_global, ifname.c_str());

		if (!wpa_s) {
			wpa_printf(MSG_ERROR, "nl80211 parent interface not found");
			aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		if (ensureConfigFileExistsAtPath(kMainlineSupplicantConfigPath) != 0) {
			wpa_printf(MSG_ERROR, "Conf file does not exists: %s",
				kMainlineSupplicantConfigPath.c_str());
			aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		if (wpa_drv_if_add(wpa_s, WPA_IF_NAN, ifaceName.c_str(), NULL, NULL, NULL,
						NULL, NULL) < 0)
		{
			wpa_printf(MSG_ERROR, "Failed to create NAN iface");
			aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		struct wpa_interface iface_params = {};
		iface_params.driver = kIfaceDriverName;
		iface_params.ifname = ifaceName.c_str();
		iface_params.confname = kMainlineSupplicantConfigPath.c_str();

		if (!wpa_supplicant_add_iface(wpa_global, &iface_params, wpa_s)) {
			wpa_printf(MSG_ERROR, "Failed to add NAN data iface");
			wpa_drv_if_remove(wpa_s, WPA_IF_NAN, ifaceName.c_str());
			aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanCreateDataInterfaceResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::deleteDataInterfaceRequestInternal(
	char16_t cmdId, const std::string& ifaceName)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant *wpa_s = wpa_supplicant_get_iface(wpa_global, ifaceName.c_str());
		if (!wpa_s) {
			wpa_printf(MSG_ERROR, "Failed to find NAN data iface.");
			aidl_manager->notifyNanDeleteDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		if (wpa_supplicant_remove_iface(wpa_global, wpa_s, 0) != 0) {
			wpa_printf(MSG_ERROR, "Failed to remove NAN data iface in wpa_supplicant.");
			aidl_manager->notifyNanDeleteDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		if (wpa_drv_if_remove(wpa_global->ifaces, WPA_IF_NAN, ifaceName.c_str()) != 0) {
			wpa_printf(MSG_ERROR, "Failed to remove NAN data iface in WPA driver.");
			aidl_manager->notifyNanDeleteDataInterfaceResponse(ifname, cmdId, nan_status);
			return;
		}

		aidl_manager->notifyNanDeleteDataInterfaceResponse(ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

static std::string vectorToHexString(const std::vector<uint8_t>& input) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (uint8_t byte : input) {
        // std::setw(2) guarantees exactly two characters per byte
        // This ensures the total length is always even.
        ss << std::setw(2) << static_cast<int>(byte);
    }

    return ss.str();
}

::ndk::ScopedAStatus NanIface::startPublishRequestInternal(
	char16_t cmdId, const NanPublishRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanStartPublishResponse(
				ifname, cmdId, nan_status, -1);
			return;
		}

		if (!validateNanPublishConfig(msg)) {
			aidl_manager->notifyNanStartPublishResponse(
				ifname, cmdId, nan_status, -1);
			return;
		}

		struct nan_publish_params params =
			convertAidlNanPublishConfigToInternal(msg);
		// To prevent hex_matching_xx is freed before passing it to wpas_nan_publish,
		// it has to be assigned outside the helper function.
		std::string hex_matching_tx = vectorToHexString(msg.baseConfig.txMatchFilter);
		std::string hex_matching_rx = vectorToHexString(msg.baseConfig.rxMatchFilter);
		params.match_filter_tx = hex_matching_tx.c_str();
		params.match_filter_rx = hex_matching_rx.c_str();

		discovery_termination_indication_ =
			!msg.baseConfig.disableDiscoveryTerminationIndication;
		match_expiration_indication_ =
			!msg.baseConfig.disableMatchExpirationIndication;
		followup_received_indication_ =
			!msg.baseConfig.disableFollowupReceivedIndication;

		// service name is guaranteed to be UTF-8, so it can be convert directly
		std::string srv_name(msg.baseConfig.serviceName.begin(), msg.baseConfig.serviceName.end());
		auto ssi_buf =
			convertVectorToWpaBuf(msg.baseConfig.serviceSpecificInfo);
		int publish_id = 0;
		publish_id = wpas_nan_publish(
			wpa_s,
			srv_name.c_str(),
			NAN_SRV_PROTO_GENERIC, ssi_buf.get(), &params, false);
		if (publish_id < 0) {
			aidl_manager->notifyNanStartPublishResponse(
				ifname, cmdId, nan_status, -1);
			return;
		}

		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanStartPublishResponse(ifname, cmdId, nan_status, publish_id);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::stopPublishRequestInternal(
	char16_t cmdId, int8_t sessionId)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::SUCCESS;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname_.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanStopPublishResponse(ifname, cmdId, nan_status);
			return;
		}

		wpas_nan_cancel_publish(wpa_s, sessionId);
		aidl_manager->notifyNanStopPublishResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::startSubscribeRequestInternal(
	char16_t cmdId, const NanSubscribeRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanStartSubscribeResponse(
				ifname, cmdId, nan_status, 0);
			return;
		}

		if (!validateNanSubscribeConfig(msg)) {
			aidl_manager->notifyNanStartSubscribeResponse(
				ifname, cmdId, nan_status, 0);
			return;
		}

		struct nan_subscribe_params params =
			convertAidlNanSubscribeConfigToInternal(msg);
		// To prevent hex_matching_xx is freed before passing it to wpas_nan_publish,
		// it has to be assigned outside the helper function.
		std::string hex_matching_tx = vectorToHexString(msg.baseConfig.txMatchFilter);
		std::string hex_matching_rx = vectorToHexString(msg.baseConfig.rxMatchFilter);
		params.match_filter_tx = hex_matching_tx.c_str();
		params.match_filter_rx = hex_matching_rx.c_str();

		// service name is guaranteed to be UTF-8, so it can be convert directly
		std::string srv_name(msg.baseConfig.serviceName.begin(), msg.baseConfig.serviceName.end());

		auto ssi_buf =
			convertVectorToWpaBuf(msg.baseConfig.serviceSpecificInfo);
		int subscribe_id = 0;
		subscribe_id = wpas_nan_subscribe(
			wpa_s,
			srv_name.c_str(),
			NAN_SRV_PROTO_GENERIC, ssi_buf.get(), &params, false);
		if (subscribe_id < 0) {
			aidl_manager->notifyNanStartSubscribeResponse(
				ifname, cmdId, nan_status, 0);
			return;
		}
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanStartSubscribeResponse(ifname, cmdId, nan_status, subscribe_id);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::stopSubscribeRequestInternal(
	char16_t cmdId, int8_t sessionId)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}
	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanStopSubscribeResponse(ifname, cmdId, nan_status);
			return;
		}

		nan_status.status = NanStatusCode::SUCCESS;
		wpas_nan_cancel_subscribe(wpa_s, sessionId);
		aidl_manager->notifyNanStopSubscribeResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::transmitFollowupRequestInternal(
	char16_t cmdId, const NanTransmitFollowupRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;

		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanTransmitFollowupResponse(ifname, cmdId, nan_status);
			return;
		}

		auto ssi_buf = convertVectorToWpaBuf(msg.serviceSpecificInfo);
		auto elems = convertVectorToWpaBuf(msg.extendedServiceSpecificInfo);
		int ret = wpas_nan_transmit(
			wpa_supplicant_get_iface(wpa_global, ifname.c_str()),
			msg.discoverySessionId, ssi_buf.get(), elems.get(),
			msg.addr.data(), msg.peerId);
		if (ret < 0) {
			aidl_manager->notifyNanTransmitFollowupResponse(
				ifname, cmdId, nan_status);
			return;
		}
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanTransmitFollowupResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::initiateBootstrappingRequestInternal(
	char16_t cmdId, const NanBootstrappingRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;

		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanInitiateBootstrappingResponse(ifname, cmdId, nan_status, 0);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for bootstrapping request initialization
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanInitiateBootstrappingResponse(ifname, cmdId, nan_status, 0);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::respondToBootstrappingIndicationRequestInternal(
	char16_t cmdId, const NanBootstrappingResponse& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;

		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanRespondToBootstrappingIndicationResponse(
				ifname, cmdId, nan_status);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for responding bootstrapping request
		nan_status.status = NanStatusCode::SUCCESS;
		aidl_manager->notifyNanRespondToBootstrappingIndicationResponse(
			ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::initiatePairingRequestInternal(
	char16_t cmdId, const NanPairingRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;

		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanInitiatePairingResponse(ifname, cmdId, nan_status, 0);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for pairing request initialization
		aidl_manager->notifyNanInitiatePairingResponse(ifname, cmdId, nan_status, 0);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::respondToPairingIndicationRequestInternal(
	char16_t cmdId, const NanRespondToPairingIndicationRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanRespondToPairingIndicationResponse(
				ifname, cmdId, nan_status);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for responding pairing request
		aidl_manager->notifyNanRespondToPairingIndicationResponse(
			ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::terminatePairingRequestInternal(
	char16_t cmdId, int32_t pairingInstanceId)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanTerminatePairingResponse(ifname, cmdId, nan_status);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for terminating pairing request
		aidl_manager->notifyNanTerminatePairingResponse(ifname, cmdId, nan_status);
	}).detach();

	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::initiateDataPathRequestInternal(
	char16_t cmdId, const NanInitiateDataPathRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;
		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanInitiateDataPathResponse(ifname, cmdId, nan_status, 0);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for initiating data path request
		aidl_manager->notifyNanInitiateDataPathResponse(ifname, cmdId, nan_status, 0);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::respondToDataPathIndicationRequestInternal(
	char16_t cmdId, const NanRespondToDataPathIndicationRequest& msg)
{
	AidlManager* aidl_manager = AidlManager::getInstance();
	if (!aidl_manager) {
		return createStatus(SupplicantStatusCode::FAILURE_UNKNOWN);
	}

	std::thread([=, ifname = ifname_, wpa_global = wpa_global_] {
		NanStatus nan_status;
		nan_status.status = NanStatusCode::INTERNAL_FAILURE;

		struct wpa_supplicant* wpa_s =
			wpa_supplicant_get_iface(wpa_global, ifname.c_str());
		if (!wpa_s) {
			aidl_manager->notifyNanRespondToDataPathIndicationResponse(
				ifname, cmdId, nan_status);
			return;
		}
		// TODO(b/460750167): wpa_supplicant API for responding data path indication request
		aidl_manager->notifyNanRespondToDataPathIndicationResponse(
			ifname, cmdId, nan_status);
	}).detach();
	return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus NanIface::terminateDataPathRequestInternal(
	char16_t cmdId, int32_t ndpInstanceId)
{
	// TODO(b/460750167): wpa_supplicant API for terminate data path request & AIDL for callback response
	return createStatus(SupplicantStatusCode::FAILURE_UNSUPPORTED);
}

bool NanIface::isStartedClusterIndicationEnabled()
{
	return started_cluster_indication_;
}

bool NanIface::isJoinedClusterIndicationEnabled()
{
	return joined_cluster_indication_;
}

bool NanIface::isDiscoveryTerminationIndicationEnabled()
{
	return discovery_termination_indication_;
}

bool NanIface::isMatchExpirationIndicationEnabled()
{
	return match_expiration_indication_;
}

bool NanIface::isFollowupReceivedIndicationEnabled()
{
	return followup_received_indication_;
}
