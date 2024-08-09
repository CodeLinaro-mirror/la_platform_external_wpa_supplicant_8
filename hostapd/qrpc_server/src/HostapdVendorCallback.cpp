/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/hostapd_vendor/hostapd_vendor_msg.h>
#include "HostapdVendorCallback.h"
#include "hostapd_event_callback.h"



namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace hostapd {

using aidl::android::hardware::wifi::hostapd::HostapdStatusCode;

::ndk::ScopedAStatus HostapdVendorCallback::onCtrlEvent(const std::string& in_ifaceName, const std::string& in_event_str)
{
	std::vector<uint8_t> data;
	if (!HostapdVendorSerializeOnCtrlEventInd(in_ifaceName,in_event_str, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onCtrlEvent>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_VENDOR_ON_CTRL_EVENT_IND, data);

}
::ndk::ScopedAStatus HostapdVendorCallback::onApInstanceInfoChanged(const ::aidl::vendor::qti::hardware::wifi::hostapd::VendorApInfo& in_apInfo)
{
	std::vector<uint8_t> data;
	if (!HostapdVendorSerializeOnApInstanceInfoChangedInd(in_apInfo, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onApInstanceInfoChanged>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_VENDOR_ON_AP_INSTANCE_INFO_CHANGED_IND, data);

}

::ndk::ScopedAStatus HostapdVendorCallback::onFailure(const std::string& in_ifname, const std::string& in_instanceName)
{
	std::vector<uint8_t> data;
    if (!HostapdVendorSerializeOnFailureInd(in_ifname, in_instanceName, data)) {
        ALOGE("[Fail] Serializing ApInstance Event <onFailure>");
        return ndk::ScopedAStatus::fail(HostapdStatusCode::FAILURE_ARGS_INVALID);
    }
    return HostapdSendEvent(HOSTAPD_VENDOR_ON_FAILURE_IND, data);
}

}  // namespace hostapd
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
