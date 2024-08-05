/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <aidl/android/hardware/wifi/supplicant/NetworkRequestEapSimGsmAuthParams.h>
#include <aidl/android/hardware/wifi/supplicant/NetworkRequestEapSimUmtsAuthParams.h>
#include <aidl/android/hardware/wifi/supplicant/TransitionDisableIndication.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class NetworkRequestEapSimGsmAuthParams;
class NetworkRequestEapSimUmtsAuthParams;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantStaNetworkCallback : public ::ndk::ICInterface {
public:
//  static bool setDefaultImpl(const std::shared_ptr<ISupplicantStaNetworkCallback>& impl);
//  static const std::shared_ptr<ISupplicantStaNetworkCallback>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus onNetworkEapIdentityRequest() = 0;
  virtual ::ndk::ScopedAStatus onNetworkEapSimGsmAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimGsmAuthParams& in_params) = 0;
  virtual ::ndk::ScopedAStatus onNetworkEapSimUmtsAuthRequest(const ::aidl::android::hardware::wifi::supplicant::NetworkRequestEapSimUmtsAuthParams& in_params) = 0;
  virtual ::ndk::ScopedAStatus onTransitionDisable(::aidl::android::hardware::wifi::supplicant::TransitionDisableIndication in_ind) = 0;
  virtual ::ndk::ScopedAStatus onServerCertificateAvailable(int32_t in_depth, const std::vector<uint8_t>& in_subject, const std::vector<uint8_t>& in_certHash, const std::vector<uint8_t>& in_certBlob) = 0;
  virtual ::ndk::ScopedAStatus onPermanentIdReqDenied() = 0;
//private:
//  static std::shared_ptr<ISupplicantStaNetworkCallback> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
