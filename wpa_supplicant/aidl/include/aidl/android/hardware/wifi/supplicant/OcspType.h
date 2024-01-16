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
enum class OcspType : int32_t {
  NONE = 0,
  REQUEST_CERT_STATUS = 1,
  REQUIRE_CERT_STATUS = 2,
  REQUIRE_ALL_CERTS_STATUS = 3,
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
[[nodiscard]] static inline std::string toString(OcspType val) {
  switch(val) {
  case OcspType::NONE:
    return "NONE";
  case OcspType::REQUEST_CERT_STATUS:
    return "REQUEST_CERT_STATUS";
  case OcspType::REQUIRE_CERT_STATUS:
    return "REQUIRE_CERT_STATUS";
  case OcspType::REQUIRE_ALL_CERTS_STATUS:
    return "REQUIRE_ALL_CERTS_STATUS";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
