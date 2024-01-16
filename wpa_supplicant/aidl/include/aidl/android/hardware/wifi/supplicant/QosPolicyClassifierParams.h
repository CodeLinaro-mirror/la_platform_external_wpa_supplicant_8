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
#include <aidl/android/hardware/wifi/supplicant/IpVersion.h>
#include <aidl/android/hardware/wifi/supplicant/PortRange.h>
#include <aidl/android/hardware/wifi/supplicant/ProtocolNextHeader.h>
#include <aidl/android/hardware/wifi/supplicant/QosPolicyClassifierParamsMask.h>

namespace aidl::android::hardware::wifi::supplicant {
class PortRange;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class QosPolicyClassifierParams {
public:
  ::aidl::android::hardware::wifi::supplicant::IpVersion ipVersion = ::aidl::android::hardware::wifi::supplicant::IpVersion(0);
  ::aidl::android::hardware::wifi::supplicant::QosPolicyClassifierParamsMask classifierParamMask = ::aidl::android::hardware::wifi::supplicant::QosPolicyClassifierParamsMask(0);
  std::vector<uint8_t> srcIp;
  std::vector<uint8_t> dstIp;
  int32_t srcPort = 0;
  ::aidl::android::hardware::wifi::supplicant::PortRange dstPortRange;
  ::aidl::android::hardware::wifi::supplicant::ProtocolNextHeader protocolNextHdr = ::aidl::android::hardware::wifi::supplicant::ProtocolNextHeader(0);
  std::vector<uint8_t> flowLabelIpv6;
  std::string domainName;
  int8_t dscp = 0;

  inline bool operator!=(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) != std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
  inline bool operator<(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) < std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
  inline bool operator<=(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) <= std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
  inline bool operator==(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) == std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
  inline bool operator>(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) > std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
  inline bool operator>=(const QosPolicyClassifierParams& rhs) const {
    return std::tie(ipVersion, classifierParamMask, srcIp, dstIp, srcPort, dstPortRange, protocolNextHdr, flowLabelIpv6, domainName, dscp) >= std::tie(rhs.ipVersion, rhs.classifierParamMask, rhs.srcIp, rhs.dstIp, rhs.srcPort, rhs.dstPortRange, rhs.protocolNextHdr, rhs.flowLabelIpv6, rhs.domainName, rhs.dscp);
  }
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
