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
#include <aidl/android/hardware/wifi/supplicant/AuthAlgMask.h>
#include <aidl/android/hardware/wifi/supplicant/DppConnectionKeys.h>
#include <aidl/android/hardware/wifi/supplicant/EapMethod.h>
#include <aidl/android/hardware/wifi/supplicant/EapPhase2Method.h>
#include <aidl/android/hardware/wifi/supplicant/GroupCipherMask.h>
#include <aidl/android/hardware/wifi/supplicant/GroupMgmtCipherMask.h>
#include <aidl/android/hardware/wifi/supplicant/ISupplicantStaNetworkCallback.h>
#include <aidl/android/hardware/wifi/supplicant/IfaceType.h>
#include <aidl/android/hardware/wifi/supplicant/KeyMgmtMask.h>
#include <aidl/android/hardware/wifi/supplicant/NetworkResponseEapSimGsmAuthParams.h>
#include <aidl/android/hardware/wifi/supplicant/NetworkResponseEapSimUmtsAuthParams.h>
#include <aidl/android/hardware/wifi/supplicant/OcspType.h>
#include <aidl/android/hardware/wifi/supplicant/PairwiseCipherMask.h>
#include <aidl/android/hardware/wifi/supplicant/ProtoMask.h>
#include <aidl/android/hardware/wifi/supplicant/SaeH2eMode.h>
#include <aidl/android/hardware/wifi/supplicant/TlsVersion.h>

#include <aidl/android/hardware/wifi/supplicant/ICInterface.h>
#include <aidl/android/hardware/wifi/supplicant/ScopedAStatus.h>

namespace aidl::android::hardware::wifi::supplicant {
class DppConnectionKeys;
class ISupplicantStaNetworkCallback;
class NetworkResponseEapSimGsmAuthParams;
class NetworkResponseEapSimUmtsAuthParams;
}  // namespace aidl::android::hardware::wifi::supplicant
namespace aidl {
namespace android {
namespace hardware {
namespace wifi {
namespace supplicant {
class ISupplicantStaNetwork : public ::ndk::ICInterface {
public:
//  ISupplicantStaNetwork();
//  virtual ~ISupplicantStaNetwork();

  enum : int32_t { SSID_MAX_LEN_IN_BYTES = 32 };
  enum : int32_t { PSK_PASSPHRASE_MIN_LEN_IN_BYTES = 8 };
  enum : int32_t { PSK_PASSPHRASE_MAX_LEN_IN_BYTES = 63 };
  enum : int32_t { WEP_KEYS_MAX_NUM = 4 };
  enum : int32_t { WEP40_KEY_LEN_IN_BYTES = 5 };
  enum : int32_t { WEP104_KEY_LEN_IN_BYTES = 13 };

  static bool setDefaultImpl(const std::shared_ptr<ISupplicantStaNetwork>& impl);
  static const std::shared_ptr<ISupplicantStaNetwork>& getDefaultImpl();
  virtual ::ndk::ScopedAStatus disable() = 0;
  virtual ::ndk::ScopedAStatus enable(bool in_noConnect) = 0;
//  virtual ::ndk::ScopedAStatus enableSaePkOnlyMode(bool in_enable) = 0;
//  virtual ::ndk::ScopedAStatus enableSuiteBEapOpenSslCiphers() = 0;
//  virtual ::ndk::ScopedAStatus enableTlsSuiteBEapPhase1Param(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus getAuthAlg(::aidl::android::hardware::wifi::supplicant::AuthAlgMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getBssid(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapAltSubjectMatch(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapAnonymousIdentity(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapCACert(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapCAPath(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapClientCert(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapDomainSuffixMatch(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapEngine(bool* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapEngineId(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapIdentity(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapMethod(::aidl::android::hardware::wifi::supplicant::EapMethod* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapPassword(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapPhase2Method(::aidl::android::hardware::wifi::supplicant::EapPhase2Method* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapPrivateKeyId(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEapSubjectMatch(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getEdmg(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getGroupCipher(::aidl::android::hardware::wifi::supplicant::GroupCipherMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getGroupMgmtCipher(::aidl::android::hardware::wifi::supplicant::GroupMgmtCipherMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getId(int32_t* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getIdStr(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getInterfaceName(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getKeyMgmt(::aidl::android::hardware::wifi::supplicant::KeyMgmtMask* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getOcsp(::aidl::android::hardware::wifi::supplicant::OcspType* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getPairwiseCipher(::aidl::android::hardware::wifi::supplicant::PairwiseCipherMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getProto(::aidl::android::hardware::wifi::supplicant::ProtoMask* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getPsk(std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getPskPassphrase(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getRequirePmf(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getSaePassword(std::string* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getSaePasswordId(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getScanSsid(bool* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getSsid(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getType(::aidl::android::hardware::wifi::supplicant::IfaceType* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getWapiCertSuite(std::string* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getWepKey(int32_t in_keyIdx, std::vector<uint8_t>* _aidl_return) = 0;
  virtual ::ndk::ScopedAStatus getWepTxKeyIdx(int32_t* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus getWpsNfcConfigurationToken(std::vector<uint8_t>* _aidl_return) = 0;
//  virtual ::ndk::ScopedAStatus registerCallback(const std::shared_ptr<::aidl::android::hardware::wifi::supplicant::ISupplicantStaNetworkCallback>& in_callback) = 0;
  virtual ::ndk::ScopedAStatus select() = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapIdentityResponse(const std::vector<uint8_t>& in_identity, const std::vector<uint8_t>& in_encryptedIdentity) = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapSimGsmAuthFailure() = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapSimGsmAuthResponse(const std::vector<::aidl::android::hardware::wifi::supplicant::NetworkResponseEapSimGsmAuthParams>& in_params) = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapSimUmtsAuthFailure() = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapSimUmtsAuthResponse(const ::aidl::android::hardware::wifi::supplicant::NetworkResponseEapSimUmtsAuthParams& in_params) = 0;
//  virtual ::ndk::ScopedAStatus sendNetworkEapSimUmtsAutsResponse(const std::vector<uint8_t>& in_auts) = 0;
  virtual ::ndk::ScopedAStatus setAuthAlg(::aidl::android::hardware::wifi::supplicant::AuthAlgMask in_authAlgMask) = 0;
  virtual ::ndk::ScopedAStatus setBssid(const std::vector<uint8_t>& in_bssid) = 0;
//  virtual ::ndk::ScopedAStatus setDppKeys(const ::aidl::android::hardware::wifi::supplicant::DppConnectionKeys& in_keys) = 0;
//  virtual ::ndk::ScopedAStatus setEapAltSubjectMatch(const std::string& in_match) = 0;
//  virtual ::ndk::ScopedAStatus setEapAnonymousIdentity(const std::vector<uint8_t>& in_identity) = 0;
//  virtual ::ndk::ScopedAStatus setEapCACert(const std::string& in_path) = 0;
//  virtual ::ndk::ScopedAStatus setEapCAPath(const std::string& in_path) = 0;
//  virtual ::ndk::ScopedAStatus setEapClientCert(const std::string& in_path) = 0;
//  virtual ::ndk::ScopedAStatus setEapDomainSuffixMatch(const std::string& in_match) = 0;
//  virtual ::ndk::ScopedAStatus setEapEncryptedImsiIdentity(const std::vector<uint8_t>& in_identity) = 0;
//  virtual ::ndk::ScopedAStatus setEapEngine(bool in_enable) = 0;
//  virtual ::ndk::ScopedAStatus setEapEngineID(const std::string& in_id) = 0;
//  virtual ::ndk::ScopedAStatus setEapErp(bool in_enable) = 0;
//  virtual ::ndk::ScopedAStatus setEapIdentity(const std::vector<uint8_t>& in_identity) = 0;
//  virtual ::ndk::ScopedAStatus setEapMethod(::aidl::android::hardware::wifi::supplicant::EapMethod in_method) = 0;
//  virtual ::ndk::ScopedAStatus setEapPassword(const std::vector<uint8_t>& in_password) = 0;
//  virtual ::ndk::ScopedAStatus setEapPhase2Method(::aidl::android::hardware::wifi::supplicant::EapPhase2Method in_method) = 0;
//  virtual ::ndk::ScopedAStatus setEapPrivateKeyId(const std::string& in_id) = 0;
//  virtual ::ndk::ScopedAStatus setEapSubjectMatch(const std::string& in_match) = 0;
//  virtual ::ndk::ScopedAStatus setEdmg(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setGroupCipher(::aidl::android::hardware::wifi::supplicant::GroupCipherMask in_groupCipherMask) = 0;
  virtual ::ndk::ScopedAStatus setGroupMgmtCipher(::aidl::android::hardware::wifi::supplicant::GroupMgmtCipherMask in_groupMgmtCipherMask) = 0;
  virtual ::ndk::ScopedAStatus setIdStr(const std::string& in_idStr) = 0;
  virtual ::ndk::ScopedAStatus setKeyMgmt(::aidl::android::hardware::wifi::supplicant::KeyMgmtMask in_keyMgmtMask) = 0;
//  virtual ::ndk::ScopedAStatus setOcsp(::aidl::android::hardware::wifi::supplicant::OcspType in_ocspType) = 0;
  virtual ::ndk::ScopedAStatus setPairwiseCipher(::aidl::android::hardware::wifi::supplicant::PairwiseCipherMask in_pairwiseCipherMask) = 0;
  virtual ::ndk::ScopedAStatus setPmkCache(const std::vector<uint8_t>& in_serializedEntry) = 0;
//  virtual ::ndk::ScopedAStatus setProactiveKeyCaching(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setProto(::aidl::android::hardware::wifi::supplicant::ProtoMask in_protoMask) = 0;
  virtual ::ndk::ScopedAStatus setPsk(const std::vector<uint8_t>& in_psk) = 0;
  virtual ::ndk::ScopedAStatus setPskPassphrase(const std::string& in_psk) = 0;
  virtual ::ndk::ScopedAStatus setRequirePmf(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setSaeH2eMode(::aidl::android::hardware::wifi::supplicant::SaeH2eMode in_mode) = 0;
  virtual ::ndk::ScopedAStatus setSaePassword(const std::string& in_saePassword) = 0;
//  virtual ::ndk::ScopedAStatus setSaePasswordId(const std::string& in_saePasswordId) = 0;
  virtual ::ndk::ScopedAStatus setScanSsid(bool in_enable) = 0;
  virtual ::ndk::ScopedAStatus setSsid(const std::vector<uint8_t>& in_ssid) = 0;
//  virtual ::ndk::ScopedAStatus setUpdateIdentifier(int32_t in_id) = 0;
//  virtual ::ndk::ScopedAStatus setWapiCertSuite(const std::string& in_suite) = 0;
  virtual ::ndk::ScopedAStatus setWepKey(int32_t in_keyIdx, const std::vector<uint8_t>& in_wepKey) = 0;
  virtual ::ndk::ScopedAStatus setWepTxKeyIdx(int32_t in_keyIdx) = 0;
//  virtual ::ndk::ScopedAStatus setRoamingConsortiumSelection(const std::vector<uint8_t>& in_selectedRcoi) = 0;
//  virtual ::ndk::ScopedAStatus setMinimumTlsVersionEapPhase1Param(::aidl::android::hardware::wifi::supplicant::TlsVersion in_tlsVersion) = 0;
//  virtual ::ndk::ScopedAStatus setStrictConservativePeerMode(bool in_enable) = 0;
private:
  static std::shared_ptr<ISupplicantStaNetwork> default_impl;
};
}  // namespace supplicant
}  // namespace wifi
}  // namespace hardware
}  // namespace android
}  // namespace aidl
