/*
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
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
enum class SupplicantVendorStatusCode : int8_t {
  SUCCESS = 0,
  FAILURE_UNKNOWN = 1,
  FAILURE_ARGS_INVALID = 2,
  FAILURE_IFACE_INVALID = 3,
  FAILURE_UNSUPPORTED = 4,
};

}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
namespace aidl {
namespace vendor {
namespace qti {
namespace hardware {
namespace wifi {
namespace supplicant {
[[nodiscard]] static inline std::string toString(SupplicantVendorStatusCode val) {
  switch(val) {
  case SupplicantVendorStatusCode::SUCCESS:
    return "SUCCESS";
  case SupplicantVendorStatusCode::FAILURE_UNKNOWN:
    return "FAILURE_UNKNOWN";
  case SupplicantVendorStatusCode::FAILURE_ARGS_INVALID:
    return "FAILURE_ARGS_INVALID";
  case SupplicantVendorStatusCode::FAILURE_IFACE_INVALID:
    return "FAILURE_IFACE_INVALID";
  case SupplicantVendorStatusCode::FAILURE_UNSUPPORTED:
    return "FAILURE_UNSUPPORTED";
  default:
    return std::to_string(static_cast<int8_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace qti
}  // namespace vendor
}  // namespace aidl
