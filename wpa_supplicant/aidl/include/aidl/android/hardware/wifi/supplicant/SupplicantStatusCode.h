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
enum class SupplicantStatusCode : int32_t {
  SUCCESS = 0,
  FAILURE_UNKNOWN = 1,
  FAILURE_ARGS_INVALID = 2,
  FAILURE_IFACE_INVALID = 3,
  FAILURE_IFACE_UNKNOWN = 4,
  FAILURE_IFACE_EXISTS = 5,
  FAILURE_IFACE_DISABLED = 6,
  FAILURE_IFACE_NOT_DISCONNECTED = 7,
  FAILURE_NETWORK_INVALID = 8,
  FAILURE_NETWORK_UNKNOWN = 9,
  FAILURE_UNSUPPORTED = 10,
  FAILURE_ONGOING_REQUEST = 11,
};

class SupplicantStatus {
public:
    SupplicantStatus() :
        code(SupplicantStatusCode::SUCCESS), description("") {}

    SupplicantStatus(const SupplicantStatusCode &_code) :
        code(_code), description("") {}

    SupplicantStatus(const SupplicantStatusCode &_code, const std::string &_description) :
        code(_code), description(_description) {}

    inline bool operator==(const SupplicantStatus& other) const {
        return (code == other.code && description == other.description);
    }
    inline bool operator!=(const SupplicantStatus &other) const {
        return !((*this) == other);
    }

    SupplicantStatusCode code;
    std::string description;
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
[[nodiscard]] static inline std::string toString(SupplicantStatusCode val) {
  switch(val) {
  case SupplicantStatusCode::SUCCESS:
    return "SUCCESS";
  case SupplicantStatusCode::FAILURE_UNKNOWN:
    return "FAILURE_UNKNOWN";
  case SupplicantStatusCode::FAILURE_ARGS_INVALID:
    return "FAILURE_ARGS_INVALID";
  case SupplicantStatusCode::FAILURE_IFACE_INVALID:
    return "FAILURE_IFACE_INVALID";
  case SupplicantStatusCode::FAILURE_IFACE_UNKNOWN:
    return "FAILURE_IFACE_UNKNOWN";
  case SupplicantStatusCode::FAILURE_IFACE_EXISTS:
    return "FAILURE_IFACE_EXISTS";
  case SupplicantStatusCode::FAILURE_IFACE_DISABLED:
    return "FAILURE_IFACE_DISABLED";
  case SupplicantStatusCode::FAILURE_IFACE_NOT_DISCONNECTED:
    return "FAILURE_IFACE_NOT_DISCONNECTED";
  case SupplicantStatusCode::FAILURE_NETWORK_INVALID:
    return "FAILURE_NETWORK_INVALID";
  case SupplicantStatusCode::FAILURE_NETWORK_UNKNOWN:
    return "FAILURE_NETWORK_UNKNOWN";
  case SupplicantStatusCode::FAILURE_UNSUPPORTED:
    return "FAILURE_UNSUPPORTED";
  case SupplicantStatusCode::FAILURE_ONGOING_REQUEST:
    return "FAILURE_ONGOING_REQUEST";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
