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
enum class StaIfaceReasonCode : int32_t {
  UNSPECIFIED = 1,
  PREV_AUTH_NOT_VALID = 2,
  DEAUTH_LEAVING = 3,
  DISASSOC_DUE_TO_INACTIVITY = 4,
  DISASSOC_AP_BUSY = 5,
  CLASS2_FRAME_FROM_NONAUTH_STA = 6,
  CLASS3_FRAME_FROM_NONASSOC_STA = 7,
  DISASSOC_STA_HAS_LEFT = 8,
  STA_REQ_ASSOC_WITHOUT_AUTH = 9,
  PWR_CAPABILITY_NOT_VALID = 10,
  SUPPORTED_CHANNEL_NOT_VALID = 11,
  BSS_TRANSITION_DISASSOC = 12,
  INVALID_IE = 13,
  MICHAEL_MIC_FAILURE = 14,
  FOURWAY_HANDSHAKE_TIMEOUT = 15,
  GROUP_KEY_UPDATE_TIMEOUT = 16,
  IE_IN_4WAY_DIFFERS = 17,
  GROUP_CIPHER_NOT_VALID = 18,
  PAIRWISE_CIPHER_NOT_VALID = 19,
  AKMP_NOT_VALID = 20,
  UNSUPPORTED_RSN_IE_VERSION = 21,
  INVALID_RSN_IE_CAPAB = 22,
  IEEE_802_1X_AUTH_FAILED = 23,
  CIPHER_SUITE_REJECTED = 24,
  TDLS_TEARDOWN_UNREACHABLE = 25,
  TDLS_TEARDOWN_UNSPECIFIED = 26,
  SSP_REQUESTED_DISASSOC = 27,
  NO_SSP_ROAMING_AGREEMENT = 28,
  BAD_CIPHER_OR_AKM = 29,
  NOT_AUTHORIZED_THIS_LOCATION = 30,
  SERVICE_CHANGE_PRECLUDES_TS = 31,
  UNSPECIFIED_QOS_REASON = 32,
  NOT_ENOUGH_BANDWIDTH = 33,
  DISASSOC_LOW_ACK = 34,
  EXCEEDED_TXOP = 35,
  STA_LEAVING = 36,
  END_TS_BA_DLS = 37,
  UNKNOWN_TS_BA = 38,
  TIMEOUT = 39,
  PEERKEY_MISMATCH = 45,
  AUTHORIZED_ACCESS_LIMIT_REACHED = 46,
  EXTERNAL_SERVICE_REQUIREMENTS = 47,
  INVALID_FT_ACTION_FRAME_COUNT = 48,
  INVALID_PMKID = 49,
  INVALID_MDE = 50,
  INVALID_FTE = 51,
  MESH_PEERING_CANCELLED = 52,
  MESH_MAX_PEERS = 53,
  MESH_CONFIG_POLICY_VIOLATION = 54,
  MESH_CLOSE_RCVD = 55,
  MESH_MAX_RETRIES = 56,
  MESH_CONFIRM_TIMEOUT = 57,
  MESH_INVALID_GTK = 58,
  MESH_INCONSISTENT_PARAMS = 59,
  MESH_INVALID_SECURITY_CAP = 60,
  MESH_PATH_ERROR_NO_PROXY_INFO = 61,
  MESH_PATH_ERROR_NO_FORWARDING_INFO = 62,
  MESH_PATH_ERROR_DEST_UNREACHABLE = 63,
  MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS = 64,
  MESH_CHANNEL_SWITCH_REGULATORY_REQ = 65,
  MESH_CHANNEL_SWITCH_UNSPECIFIED = 66,
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
[[nodiscard]] static inline std::string toString(StaIfaceReasonCode val) {
  switch(val) {
  case StaIfaceReasonCode::UNSPECIFIED:
    return "UNSPECIFIED";
  case StaIfaceReasonCode::PREV_AUTH_NOT_VALID:
    return "PREV_AUTH_NOT_VALID";
  case StaIfaceReasonCode::DEAUTH_LEAVING:
    return "DEAUTH_LEAVING";
  case StaIfaceReasonCode::DISASSOC_DUE_TO_INACTIVITY:
    return "DISASSOC_DUE_TO_INACTIVITY";
  case StaIfaceReasonCode::DISASSOC_AP_BUSY:
    return "DISASSOC_AP_BUSY";
  case StaIfaceReasonCode::CLASS2_FRAME_FROM_NONAUTH_STA:
    return "CLASS2_FRAME_FROM_NONAUTH_STA";
  case StaIfaceReasonCode::CLASS3_FRAME_FROM_NONASSOC_STA:
    return "CLASS3_FRAME_FROM_NONASSOC_STA";
  case StaIfaceReasonCode::DISASSOC_STA_HAS_LEFT:
    return "DISASSOC_STA_HAS_LEFT";
  case StaIfaceReasonCode::STA_REQ_ASSOC_WITHOUT_AUTH:
    return "STA_REQ_ASSOC_WITHOUT_AUTH";
  case StaIfaceReasonCode::PWR_CAPABILITY_NOT_VALID:
    return "PWR_CAPABILITY_NOT_VALID";
  case StaIfaceReasonCode::SUPPORTED_CHANNEL_NOT_VALID:
    return "SUPPORTED_CHANNEL_NOT_VALID";
  case StaIfaceReasonCode::BSS_TRANSITION_DISASSOC:
    return "BSS_TRANSITION_DISASSOC";
  case StaIfaceReasonCode::INVALID_IE:
    return "INVALID_IE";
  case StaIfaceReasonCode::MICHAEL_MIC_FAILURE:
    return "MICHAEL_MIC_FAILURE";
  case StaIfaceReasonCode::FOURWAY_HANDSHAKE_TIMEOUT:
    return "FOURWAY_HANDSHAKE_TIMEOUT";
  case StaIfaceReasonCode::GROUP_KEY_UPDATE_TIMEOUT:
    return "GROUP_KEY_UPDATE_TIMEOUT";
  case StaIfaceReasonCode::IE_IN_4WAY_DIFFERS:
    return "IE_IN_4WAY_DIFFERS";
  case StaIfaceReasonCode::GROUP_CIPHER_NOT_VALID:
    return "GROUP_CIPHER_NOT_VALID";
  case StaIfaceReasonCode::PAIRWISE_CIPHER_NOT_VALID:
    return "PAIRWISE_CIPHER_NOT_VALID";
  case StaIfaceReasonCode::AKMP_NOT_VALID:
    return "AKMP_NOT_VALID";
  case StaIfaceReasonCode::UNSUPPORTED_RSN_IE_VERSION:
    return "UNSUPPORTED_RSN_IE_VERSION";
  case StaIfaceReasonCode::INVALID_RSN_IE_CAPAB:
    return "INVALID_RSN_IE_CAPAB";
  case StaIfaceReasonCode::IEEE_802_1X_AUTH_FAILED:
    return "IEEE_802_1X_AUTH_FAILED";
  case StaIfaceReasonCode::CIPHER_SUITE_REJECTED:
    return "CIPHER_SUITE_REJECTED";
  case StaIfaceReasonCode::TDLS_TEARDOWN_UNREACHABLE:
    return "TDLS_TEARDOWN_UNREACHABLE";
  case StaIfaceReasonCode::TDLS_TEARDOWN_UNSPECIFIED:
    return "TDLS_TEARDOWN_UNSPECIFIED";
  case StaIfaceReasonCode::SSP_REQUESTED_DISASSOC:
    return "SSP_REQUESTED_DISASSOC";
  case StaIfaceReasonCode::NO_SSP_ROAMING_AGREEMENT:
    return "NO_SSP_ROAMING_AGREEMENT";
  case StaIfaceReasonCode::BAD_CIPHER_OR_AKM:
    return "BAD_CIPHER_OR_AKM";
  case StaIfaceReasonCode::NOT_AUTHORIZED_THIS_LOCATION:
    return "NOT_AUTHORIZED_THIS_LOCATION";
  case StaIfaceReasonCode::SERVICE_CHANGE_PRECLUDES_TS:
    return "SERVICE_CHANGE_PRECLUDES_TS";
  case StaIfaceReasonCode::UNSPECIFIED_QOS_REASON:
    return "UNSPECIFIED_QOS_REASON";
  case StaIfaceReasonCode::NOT_ENOUGH_BANDWIDTH:
    return "NOT_ENOUGH_BANDWIDTH";
  case StaIfaceReasonCode::DISASSOC_LOW_ACK:
    return "DISASSOC_LOW_ACK";
  case StaIfaceReasonCode::EXCEEDED_TXOP:
    return "EXCEEDED_TXOP";
  case StaIfaceReasonCode::STA_LEAVING:
    return "STA_LEAVING";
  case StaIfaceReasonCode::END_TS_BA_DLS:
    return "END_TS_BA_DLS";
  case StaIfaceReasonCode::UNKNOWN_TS_BA:
    return "UNKNOWN_TS_BA";
  case StaIfaceReasonCode::TIMEOUT:
    return "TIMEOUT";
  case StaIfaceReasonCode::PEERKEY_MISMATCH:
    return "PEERKEY_MISMATCH";
  case StaIfaceReasonCode::AUTHORIZED_ACCESS_LIMIT_REACHED:
    return "AUTHORIZED_ACCESS_LIMIT_REACHED";
  case StaIfaceReasonCode::EXTERNAL_SERVICE_REQUIREMENTS:
    return "EXTERNAL_SERVICE_REQUIREMENTS";
  case StaIfaceReasonCode::INVALID_FT_ACTION_FRAME_COUNT:
    return "INVALID_FT_ACTION_FRAME_COUNT";
  case StaIfaceReasonCode::INVALID_PMKID:
    return "INVALID_PMKID";
  case StaIfaceReasonCode::INVALID_MDE:
    return "INVALID_MDE";
  case StaIfaceReasonCode::INVALID_FTE:
    return "INVALID_FTE";
  case StaIfaceReasonCode::MESH_PEERING_CANCELLED:
    return "MESH_PEERING_CANCELLED";
  case StaIfaceReasonCode::MESH_MAX_PEERS:
    return "MESH_MAX_PEERS";
  case StaIfaceReasonCode::MESH_CONFIG_POLICY_VIOLATION:
    return "MESH_CONFIG_POLICY_VIOLATION";
  case StaIfaceReasonCode::MESH_CLOSE_RCVD:
    return "MESH_CLOSE_RCVD";
  case StaIfaceReasonCode::MESH_MAX_RETRIES:
    return "MESH_MAX_RETRIES";
  case StaIfaceReasonCode::MESH_CONFIRM_TIMEOUT:
    return "MESH_CONFIRM_TIMEOUT";
  case StaIfaceReasonCode::MESH_INVALID_GTK:
    return "MESH_INVALID_GTK";
  case StaIfaceReasonCode::MESH_INCONSISTENT_PARAMS:
    return "MESH_INCONSISTENT_PARAMS";
  case StaIfaceReasonCode::MESH_INVALID_SECURITY_CAP:
    return "MESH_INVALID_SECURITY_CAP";
  case StaIfaceReasonCode::MESH_PATH_ERROR_NO_PROXY_INFO:
    return "MESH_PATH_ERROR_NO_PROXY_INFO";
  case StaIfaceReasonCode::MESH_PATH_ERROR_NO_FORWARDING_INFO:
    return "MESH_PATH_ERROR_NO_FORWARDING_INFO";
  case StaIfaceReasonCode::MESH_PATH_ERROR_DEST_UNREACHABLE:
    return "MESH_PATH_ERROR_DEST_UNREACHABLE";
  case StaIfaceReasonCode::MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS:
    return "MAC_ADDRESS_ALREADY_EXISTS_IN_MBSS";
  case StaIfaceReasonCode::MESH_CHANNEL_SWITCH_REGULATORY_REQ:
    return "MESH_CHANNEL_SWITCH_REGULATORY_REQ";
  case StaIfaceReasonCode::MESH_CHANNEL_SWITCH_UNSPECIFIED:
    return "MESH_CHANNEL_SWITCH_UNSPECIFIED";
  default:
    return std::to_string(static_cast<int32_t>(val));
  }
}
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
