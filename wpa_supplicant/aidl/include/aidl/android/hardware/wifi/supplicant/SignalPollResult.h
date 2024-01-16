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
class SignalPollResult {
public:
  int32_t linkId = 0;
  int32_t currentRssiDbm = 0;
  int32_t txBitrateMbps = 0;
  int32_t rxBitrateMbps = 0;
  int32_t frequencyMhz = 0;

  inline bool operator!=(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) != std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
  inline bool operator<(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) < std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
  inline bool operator<=(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) <= std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
  inline bool operator==(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) == std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
  inline bool operator>(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) > std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
  inline bool operator>=(const SignalPollResult& rhs) const {
    return std::tie(linkId, currentRssiDbm, txBitrateMbps, rxBitrateMbps, frequencyMhz) >= std::tie(rhs.linkId, rhs.currentRssiDbm, rhs.txBitrateMbps, rhs.rxBitrateMbps, rhs.frequencyMhz);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
