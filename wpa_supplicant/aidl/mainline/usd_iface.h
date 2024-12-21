/*
 * WPA Supplicant - Interface for USD operations
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MAINLINE_SUPPLICANT_USD_IFACE_H
#define MAINLINE_SUPPLICANT_USD_IFACE_H

#include <aidl/android/system/wifi/mainline_supplicant/BnUsdInterface.h>

using ::aidl::android::system::wifi::mainline_supplicant::BnUsdInterface;
using ::aidl::android::system::wifi::mainline_supplicant::UsdMessageInfo;

class UsdIface : public BnUsdInterface {
    public:
        UsdIface(struct wpa_global* wpa_global, std::string iface_name);
        ::ndk::ScopedAStatus getUsdCapabilities(UsdCapabilities* _aidl_return) override;
        ::ndk::ScopedAStatus startUsdPublish(int32_t in_cmdId,
            const PublishConfig& in_usdPublishConfig) override;
        ::ndk::ScopedAStatus startUsdSubscribe(int32_t in_cmdId,
            const SubscribeConfig& in_usdSubscribeConfig) override;
        ::ndk::ScopedAStatus updateUsdPublish(int32_t in_publishId,
            const std::vector<uint8_t>& in_serviceSpecificInfo) override;
        ::ndk::ScopedAStatus cancelUsdPublish(int32_t in_publishId) override;
        ::ndk::ScopedAStatus cancelUsdSubscribe(int32_t in_subscribeId) override;
        ::ndk::ScopedAStatus sendUsdMessage(const UsdMessageInfo& in_messageInfo) override;

    private:
        wpa_global* wpa_global_;
        std::string iface_name_;
};

#endif // MAINLINE_SUPPLICANT_USD_IFACE_H
