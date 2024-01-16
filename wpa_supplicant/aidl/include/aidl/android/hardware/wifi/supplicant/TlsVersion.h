/*
 * Changes from Qualcomm Innovation Center, Inc. are provided under the following license:
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <array>
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
enum class TlsVersion : int32_t {
  TLS_V1_0 = 0,
  TLS_V1_1 = 1,
  TLS_V1_2 = 2,
  TLS_V1_3 = 3,
};

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
[[nodiscard]] static inline std::string toString(TlsVersion val) {
  switch(val) {
  case TlsVersion::TLS_V1_0:
    return "TLS_V1_0";
  case TlsVersion::TLS_V1_1:
    return "TLS_V1_1";
  case TlsVersion::TLS_V1_2:
    return "TLS_V1_2";
  case TlsVersion::TLS_V1_3:
    return "TLS_V1_3";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
