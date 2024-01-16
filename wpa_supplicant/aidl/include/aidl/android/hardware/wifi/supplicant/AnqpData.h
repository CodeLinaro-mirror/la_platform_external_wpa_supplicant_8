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
class AnqpData {
public:
  std::vector<uint8_t> venueName;
  std::vector<uint8_t> roamingConsortium;
  std::vector<uint8_t> ipAddrTypeAvailability;
  std::vector<uint8_t> naiRealm;
  std::vector<uint8_t> anqp3gppCellularNetwork;
  std::vector<uint8_t> domainName;
  std::vector<uint8_t> venueUrl;

  inline bool operator!=(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) != std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
  inline bool operator<(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) < std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
  inline bool operator<=(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) <= std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
  inline bool operator==(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) == std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
  inline bool operator>(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) > std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
  inline bool operator>=(const AnqpData& rhs) const {
    return std::tie(venueName, roamingConsortium, ipAddrTypeAvailability, naiRealm, anqp3gppCellularNetwork, domainName, venueUrl) >= std::tie(rhs.venueName, rhs.roamingConsortium, rhs.ipAddrTypeAvailability, rhs.naiRealm, rhs.anqp3gppCellularNetwork, rhs.domainName, rhs.venueUrl);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
