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

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class INonStandardCertCallback : public ::ndk::ICInterface {
public:
  static bool setDefaultImpl(const std::shared_ptr<INonStandardCertCallback>& impl);
  static const std::shared_ptr<INonStandardCertCallback>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus getBlob(const std::string& in_alias, std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus listAliases(const std::string& in_prefix, std::vector<std::string>* _aidl_return) = 0;
private:
  static std::shared_ptr<INonStandardCertCallback> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
