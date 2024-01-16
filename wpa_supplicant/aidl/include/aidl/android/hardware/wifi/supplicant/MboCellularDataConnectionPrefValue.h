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
enum class MboCellularDataConnectionPrefValue : int32_t {
  EXCLUDED = 0,
  NOT_PREFERRED = 1,
  PREFERRED = 255,
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
[[nodiscard]] static inline std::string toString(MboCellularDataConnectionPrefValue val) {
  switch(val) {
  case MboCellularDataConnectionPrefValue::EXCLUDED:
    return "EXCLUDED";
  case MboCellularDataConnectionPrefValue::NOT_PREFERRED:
    return "NOT_PREFERRED";
  case MboCellularDataConnectionPrefValue::PREFERRED:
    return "PREFERRED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
