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
class ISupplicantCallback : public ::ndk::ICInterface {
public:
  static bool setDefaultImpl(const std::shared_ptr<ISupplicantCallback>& impl);
  static const std::shared_ptr<ISupplicantCallback>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus onInterfaceCreated(const std::string& in_ifaceName) = 0;
  virtual ::ndk::ScopedAStatus onInterfaceRemoved(const std::string& in_ifaceName) = 0;
private:
  static std::shared_ptr<ISupplicantCallback> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
