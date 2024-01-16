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
#include <aidl/android/hardware/wifi/supplicant/GsmRand.h>

namespace aidl::android::hardware::wifi::supplicant {
class GsmRand;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class NetworkRequestEapSimGsmAuthParams {
public:
  std::vector<::aidl::android::hardware::wifi::supplicant::GsmRand> rands;

  inline bool operator!=(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) != std::tie(rhs.rands);
  }
  inline bool operator<(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) < std::tie(rhs.rands);
  }
  inline bool operator<=(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) <= std::tie(rhs.rands);
  }
  inline bool operator==(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) == std::tie(rhs.rands);
  }
  inline bool operator>(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) > std::tie(rhs.rands);
  }
  inline bool operator>=(const NetworkRequestEapSimGsmAuthParams& rhs) const {
    return std::tie(rands) >= std::tie(rhs.rands);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
