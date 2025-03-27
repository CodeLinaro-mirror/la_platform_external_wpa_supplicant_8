/*
 * WPA Supplicant - Station mode interface
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "sta_iface.h"

StaIface::StaIface(struct wpa_global* wpa_global, std::string iface_name)
    : wpa_global_(wpa_global), iface_name_(iface_name) {}

::ndk::ScopedAStatus StaIface::registerCallback(
        const std::shared_ptr<IStaInterfaceCallback>& in_callback) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::getUsdCapabilities(UsdCapabilities* _aidl_return) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::startUsdPublish(int32_t in_cmdId,
        const PublishConfig& in_usdPublishConfig) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::startUsdSubscribe(int32_t in_cmdId,
        const SubscribeConfig& in_usdSubscribeConfig) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::updateUsdPublish(int32_t in_publishId,
        const std::vector<uint8_t>& in_serviceSpecificInfo) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::cancelUsdPublish(int32_t in_publishId) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::cancelUsdSubscribe(int32_t in_subscribeId) {
    return ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus StaIface::sendUsdMessage(const UsdMessageInfo& in_messageInfo) {
    return ndk::ScopedAStatus::ok();
}
