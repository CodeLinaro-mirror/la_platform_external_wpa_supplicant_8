/*
 * Copyright (C) 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstring>

#include <gtest/gtest.h>
#include "../hostapd.cpp"

namespace aidl::android::hardware::wifi::hostapd {

/**
 * Null hostapd_data* and null mac address (u8*)
 * There's an || check on these that should return nullopt
 */
TEST(getStaInfoByMacAddrTest, NullArguments) {
	EXPECT_EQ(std::nullopt, getStaInfoByMacAddr(nullptr, nullptr));
}


/**
 * We pass valid arguments to get past the nullptr check, but hostapd_data->sta_list is nullptr.
 * Don't loop through the sta_info* list, just return nullopt.
 */
TEST(getStaInfoByMacAddrTest, NullStaList) {
	struct hostapd_data iface_hapd = {};
	u8 mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xD0, 0x0D};
	EXPECT_EQ(std::nullopt, getStaInfoByMacAddr(&iface_hapd, mac_addr));
}

/**
 * Mac doesn't match, and we hit the end of the sta_info list.
 * Don't run over the end of the list and return nullopt.
 */
TEST(getStaInfoByMacAddrTest, NoMatchingMac) {
	struct hostapd_data iface_hapd = {};
	struct sta_info sta0 = {};
	struct sta_info sta1 = {};
	struct sta_info sta2 = {};
	iface_hapd.sta_list = &sta0;
	sta0.next = &sta1;
	sta1.next = &sta2;
	u8 mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xD0, 0x0D};
	EXPECT_EQ(std::nullopt, getStaInfoByMacAddr(&iface_hapd, mac_addr));
}

/**
 * There is a matching address and we return it.
 */
TEST(getStaInfoByMacAddr, MatchingMac) {
	struct hostapd_data iface_hapd = {};
	struct sta_info sta0 = {};
	struct sta_info sta1 = {};
	struct sta_info sta2 = {};
	iface_hapd.sta_list = &sta0;
	sta0.next = &sta1;
	sta1.next = &sta2;
	u8 sta0_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xD0, 0x0C};  // off by 1 bit
	std::memcpy(sta0.addr, sta0_addr, ETH_ALEN);
	u8 sta1_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xD0, 0x0D};
	std::memcpy(sta1.addr, sta1_addr, ETH_ALEN);
	u8 mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xD0, 0x0D};
	auto sta_ptr_optional = getStaInfoByMacAddr(&iface_hapd, mac_addr);
	EXPECT_TRUE(sta_ptr_optional.has_value());
	EXPECT_EQ(0, std::memcmp(sta_ptr_optional.value()->addr, sta1_addr, ETH_ALEN));
}

}  // namespace aidl::android::hardware::wifi::hostapd
