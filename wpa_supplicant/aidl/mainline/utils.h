/*
 * WPA Supplicant - Utilities for the mainline supplicant
 * Copyright (c) 2024, Google Inc. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MAINLINE_SUPPLICANT_UTILS_H
#define MAINLINE_SUPPLICANT_UTILS_H

#include <aidl/android/system/wifi/mainline_supplicant/SupplicantStatusCode.h>

extern "C"
{
#include "utils/common.h"
#include "wpabuf.h"
}

namespace {
// Custom deleter for wpabuf
void freeWpaBuf(wpabuf *ptr) { wpabuf_free(ptr); }
}

using ::aidl::android::system::wifi::mainline_supplicant::SupplicantStatusCode;

using wpabuf_unique_ptr = std::unique_ptr<wpabuf, void (*)(wpabuf *)>;

inline ndk::ScopedAStatus createStatus(SupplicantStatusCode statusCode) {
    return ndk::ScopedAStatus::fromServiceSpecificError(
        static_cast<int32_t>(statusCode));
}

inline ndk::ScopedAStatus createStatusWithMsg(
        SupplicantStatusCode statusCode, std::string msg) {
    return ndk::ScopedAStatus::fromServiceSpecificErrorWithMessage(
        static_cast<int32_t>(statusCode), msg.c_str());
}

// Check whether the container is within the maximum size
template <typename T>
inline bool checkContainerSize(const T& container, int maxSize) {
    return container.size() <= maxSize;
}

// Check whether the enum value is within the specified range
template <typename T>
inline bool isValidEnumValue(T value, T enumRangeMin, T enumRangeMax) {
    return static_cast<uint32_t>(value) >= static_cast<uint32_t>(enumRangeMin)
        && static_cast<uint32_t>(value) <= static_cast<uint32_t>(enumRangeMax);
}

// Create a unique_ptr for a wpabuf ptr with a custom deleter
inline wpabuf_unique_ptr createWpaBufUniquePtr(struct wpabuf *raw_ptr) {
    return {raw_ptr, freeWpaBuf};
}

// Create a wpabuf ptr with a custom deleter, copying the data from the provided vector
inline wpabuf_unique_ptr convertVectorToWpaBuf(const std::vector<uint8_t> &data) {
    return createWpaBufUniquePtr(wpabuf_alloc_copy(data.data(), data.size()));
}

#endif // MAINLINE_SUPPLICANT_UTILS_H
