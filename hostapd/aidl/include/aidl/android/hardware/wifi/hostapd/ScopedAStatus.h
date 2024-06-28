/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SCOPEDSTATUS_H
#define SCOPEDSTATUS_H

#include <aidl/android/hardware/wifi/hostapd/HostapdStatusCode.h>

namespace ndk {
using aidl::android::hardware::wifi::hostapd::HostapdStatusCode;
using aidl::android::hardware::wifi::hostapd::HostapdStatus;
class ScopedAStatus : public HostapdStatus {
public:
  ScopedAStatus() : HostapdStatus() {}
  ScopedAStatus(HostapdStatusCode code) : HostapdStatus(code) {}
  ScopedAStatus(const HostapdStatusCode code, const std::string &desc)
    : HostapdStatus(code, desc) {}
  ScopedAStatus(int32_t code, const char* desc)
    : HostapdStatus(HostapdStatusCode(code), desc) {}

  static ScopedAStatus ok() {
    return ScopedAStatus();
  }

  static ScopedAStatus fail(HostapdStatusCode failCode) {
    return ScopedAStatus(failCode);
  }

  bool isOk() {
    return code == HostapdStatusCode::SUCCESS;
  }

  static ScopedAStatus fromServiceSpecificError(int32_t code) {
    return ScopedAStatus(code, "");
  }

  static ScopedAStatus fromServiceSpecificErrorWithMessage(
    int32_t code, const char* description) {
    return ScopedAStatus(code, description);
  }

  HostapdStatusCode getServiceSpecificError() {
    return code;
  }
};
} /* ndk */

/* Alias name from auto generated paramter structure */
using binder_status_t = ndk::ScopedAStatus;

/* Create default HostapdStatus Success state */
#define STATUS_OK (ndk::ScopedAStatus::ok())

#endif /* SCOPEDSTATUS_H */
