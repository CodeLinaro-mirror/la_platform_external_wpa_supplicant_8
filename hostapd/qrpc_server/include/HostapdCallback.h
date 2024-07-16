/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "aidl/android/hardware/wifi/hostapd/IHostapdCallback.h"

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace hostapd {
class HostapdCallback : public IHostapdCallback {
public:
    HostapdCallback() {}
    ~HostapdCallback() {}
    ::ndk::ScopedAStatus onApInstanceInfoChanged(const ::aidl::android::hardware::wifi::hostapd::ApInfo& in_apInfo) override;
    ::ndk::ScopedAStatus onConnectedClientsChanged(const ::aidl::android::hardware::wifi::hostapd::ClientInfo& in_clientInfo) override;
    ::ndk::ScopedAStatus onFailure(const std::string& in_ifaceName, const std::string& in_instanceName) override;
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
