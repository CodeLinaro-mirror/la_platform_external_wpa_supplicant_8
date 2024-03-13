/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "aidl/android/hardware/wifi/supplicant/ISupplicantStaNetworkCallback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class SupplicantStaNetworkCallback : public ISupplicantStaNetworkCallback {
public:
	SupplicantStaNetworkCallback(int32_t iface_instance_id, int32_t network_instance_id)
	: iface_instance_id_(iface_instance_id),
	  network_instance_id_(network_instance_id)
	{}
	~SupplicantStaNetworkCallback() {}

	int32_t getIfaceInstanceId();
	int32_t getNetworkInstanceId();

	::ndk::ScopedAStatus onNetworkEapIdentityRequest() override;
	::ndk::ScopedAStatus onNetworkEapSimGsmAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimGsmAuthParams& in_params) override;
	::ndk::ScopedAStatus onNetworkEapSimUmtsAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimUmtsAuthParams& in_params) override;
	::ndk::ScopedAStatus onTransitionDisable(::aidl::android::hardware::wifi::supplicant::TransitionDisableIndication in_ind) override;
	::ndk::ScopedAStatus onServerCertificateAvailable(int32_t in_depth, const std::vector<uint8_t>& in_subject, const std::vector<uint8_t>& in_certHash, const std::vector<uint8_t>& in_certBlob) override;
	::ndk::ScopedAStatus onPermanentIdReqDenied() override;

private:
	const int32_t iface_instance_id_;
	const int32_t network_instance_id_;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
