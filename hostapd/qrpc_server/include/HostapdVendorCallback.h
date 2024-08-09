/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include "aidl/vendor/qti/hardware/wifi/hostapd/IHostapdVendorCallback.h"

namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace hostapd {

class HostapdVendorCallback : public IHostapdVendorCallback {
public:
    HostapdVendorCallback() {}
    ~HostapdVendorCallback() {}
    ::ndk::ScopedAStatus onCtrlEvent(const std::string& in_ifaceName, const std::string& in_event_str) override;
    ::ndk::ScopedAStatus onApInstanceInfoChanged(const ::aidl::vendor::qti::hardware::wifi::hostapd::VendorApInfo& in_apInfo) override;
    ::ndk::ScopedAStatus onFailure(const std::string& in_ifname, const std::string& in_instanceName) override;
};
}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
