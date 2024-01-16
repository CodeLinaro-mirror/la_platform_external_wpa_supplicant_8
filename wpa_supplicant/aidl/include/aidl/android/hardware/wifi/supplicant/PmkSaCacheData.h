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

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class PmkSaCacheData {
public:
  std::array<uint8_t, 6> bssid = {{}};
  int64_t expirationTimeInSec = 0L;
  std::vector<uint8_t> serializedEntry;

  inline bool operator!=(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) != std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
  inline bool operator<(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) < std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
  inline bool operator<=(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) <= std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
  inline bool operator==(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) == std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
  inline bool operator>(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) > std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
  inline bool operator>=(const PmkSaCacheData& rhs) const {
    return std::tie(bssid, expirationTimeInSec, serializedEntry) >= std::tie(rhs.bssid, rhs.expirationTimeInSec, rhs.serializedEntry);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
