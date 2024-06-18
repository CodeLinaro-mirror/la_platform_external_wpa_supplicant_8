/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#pragma once

#include <optional>

#include <rpc/util/someip_util.h>

using qti::hal::rpc::SomeipMessage;

std::optional<std::vector<uint8_t>> NonStdGetCertificate(const std::string& alias);
std::optional<std::vector<std::string>> NonStdListAliases(const std::string& prefix);

std::shared_ptr<SomeipMessage> someip_send_nonstd_event(uint16_t method_id, const std::vector<uint8_t>& data, uint16_t unlock_id);