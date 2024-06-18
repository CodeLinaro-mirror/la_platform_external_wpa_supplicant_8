/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <rpc/message/wpa_supplicant/non_standard_cert_msg.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "SupplicantNonStdCallback.h"
#include "supplicant_event_callback.h"

static inline void v2s(const vector<uint8_t>& v, string& s)
{
    std::stringstream buffer;
    buffer << " ";
    for (const auto& value : v) {
        buffer << std::setfill('0') << std::setw(2) << std::hex << value+0 << " ";
    }
    s = buffer.str();
}

std::optional<std::vector<uint8_t>> NonStdGetCertificate(const std::string& alias)
{
    ALOGI("Sending Supplicant Non-Standard event <getBlob>: %s", alias.c_str());

    std::vector<uint8_t> data;
    if (!NonStandardCertSerializeGetBlobReq(alias, data)) {
        ALOGE("[Fail] Serializing Supplicant Non-Standard event <getBlob>");
        return std::nullopt;
    }

    auto req = someip_send_nonstd_event(NON_STANDARD_CERT_GET_BLOB_IND, data, NON_STANDARD_CERT_GET_BLOB_REQ);
    if (!req) {
        ALOGE("[Fail] No request received for <getBlob>");
        return std::nullopt;
    }

    std::vector<uint8_t> blob;
    if (!NonStandardCertParseGetBlobCfm(req->getData(), req->getLength(), blob)){
        ALOGE("[Fail] Parsing <getBlob> request");
        return std::nullopt;
    }

    if (!blob.size()){
        ALOGE("[Fail] Empty <getBlob> resquest");
        return std::nullopt;
    }

    ALOGI("Received <getBlob> request with len (%d)", blob.size());

    std::optional<std::vector<uint8_t>> result = blob;

    return result;
}

std::optional<std::vector<std::string>> NonStdListAliases(const std::string& prefix)
{
    ALOGI("Sending Supplicant Non-Standard event <listAliases>: %s", prefix.c_str());

    std::vector<uint8_t> data;
    if (!NonStandardCertSerializeListAliasesReq(prefix, data)) {
        ALOGE("[Fail] Serializing Supplicant Non-Standard event <listAliases>");
        return std::nullopt;
    }

    auto req = someip_send_nonstd_event(NON_STANDARD_CERT_LIST_ALIASES_IND, data, NON_STANDARD_CERT_LIST_ALIASES_REQ);
    if (!req) {
        ALOGE("[Fail] No request received for <listAliases>");
        return std::nullopt;
    }

    std::vector<std::string> aliases;
    if (!NonStandardCertParseListAliasesCfm(req->getData(), req->getLength(), aliases)){
        ALOGE("[Fail] Parsing <listAliases> resquest");
        return std::nullopt;
    }

    if (!aliases.size()){
        ALOGE("[Fail] Empty <listAliases> resquest");
        return std::nullopt;
    }

    ALOGI("Received <listAliases> request");

    std::optional<std::vector<std::string>> result = aliases;

    return result;
}
