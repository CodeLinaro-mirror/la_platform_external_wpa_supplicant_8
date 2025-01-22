/*
 ** Copyright (c) 2025 Qualcomm Innovation Center, Inc. All rights reserved.
 ** SPDX-License-Identifier: BSD-3-Clause-Clear
 **/
#include <fuzzbinder/libbinder_ndk_driver.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <android/binder_interface_utils.h>
#include "hostapd_vendor.h"
#include "config_file.h"
#include "ctrl_iface.h"
#include "ap/dpp_hostapd.h"
#include <android-base/stringprintf.h>
#include <android-base/file.h>
#include <filesystem>

using android::base::WriteStringToFile;
using android::base::StringPrintf;
using aidl::vendor::qti::hardware::wifi::hostapd::HostapdVendor;

std::shared_ptr<HostapdVendor> service_vendor;

#ifdef CONFIG_AIDL_FUZZING
struct hapd_interfaces interfaces;

/*
 * This conf_params is for reference, used when hostapd_fuzzer_wlan0.conf does
 * not exist. Testers can modify this file based on their actual testing needs.
 */
const auto conf_params =
        "interface=wlan0\n"
        "driver=nl80211\n"
        "ctrl_interface=/data/vendor/wifi/hostapd/ctrl\n"
        "ssid2=416e64726f696441505f36313738\n"
        "channel=0\n"
        "acs_exclude_dfs=1\n"
        "freqlist=2412-2484,5160-5885\n"
        "ieee80211n=1\n"
        "ieee80211ac=1\n\n\n"

        "hw_mode=any\n"
        "ht_capab=[HT40+]\n"
        "vht_oper_chwidth=1\n\n"

        "ignore_broadcast_ssid=0\n"
        "wowlan_triggers=any\n"
        "interworking=1\n"
        "access_network_type=2\n\n"

        "wpa=2\n"
        "rsn_pairwise=CCMP\n"
        "wpa_passphrase=111111111\n";

extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv) {
    constexpr char kConfFileNameFmt[] =
        "/data/vendor/wifi/hostapd/hostapd_fuzzer_%s.conf";
    /*
     * Currently, iface only supports wlan0
     */
    const std:: string iface = "wlan0";
    const std::string conf_file_path =
        StringPrintf(kConfFileNameFmt, iface.c_str());

    /*
     * Examine whether hostapd_fuzzer_wlan0.conf exists in the path
     * /data/vendor/wifi/hostapd first. If it does not exist, create
     * a new one, using the default config. If it exists, use the
     * existing one. Testers can modify this conf file themselves to
     * carry out more comprehensive tests.
     */
    if (std::filesystem::exists(conf_file_path)) {
        wpa_printf(MSG_INFO, "hostapd_fuzzer_wlan0.conf exists, use this existing conf");
    } else {
        wpa_printf(MSG_INFO, "hostapd_fuzzer_wlan0.conf does not exist, create a new one");
        if (!WriteStringToFile(
            conf_params, conf_file_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP,
            getuid(), getgid())) {
            wpa_printf(MSG_ERROR, "Failed to WriteStringToFile");
            return -1;
        }
    }

    std::string add_iface_param_str = StringPrintf(
        "%s config=%s", iface.c_str(),
        conf_file_path.c_str());
    std::vector<char> add_iface_param_vec(
        add_iface_param_str.begin(), add_iface_param_str.end() + 1);

    wpa_printf(MSG_INFO, "conf_file_path=%s, "
        "add_iface_param_vec.data()=%s, "
        "add_iface_param_str.c_str()=%s",
        conf_file_path.c_str(),
        add_iface_param_vec.data(),
        add_iface_param_str.c_str());

    os_memset(&interfaces, 0, sizeof(interfaces));
    if (hostapd_add_iface(&interfaces, add_iface_param_vec.data())) {
        wpa_printf(
            MSG_ERROR, "Adding interface fail");
        return -1;
    }

    service_vendor = ndk::SharedRefBase::make<HostapdVendor>(&interfaces);
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (service_vendor == nullptr) {
        return -1;
    }
    android::fuzzService(service_vendor->asBinder().get(), FuzzedDataProvider(data, size));
    return 0;
}
#endif
