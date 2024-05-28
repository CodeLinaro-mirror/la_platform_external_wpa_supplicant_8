/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef SUPPLICANT_MESSAGE_HANDLER_H
#define SUPPLICANT_MESSAGE_HANDLER_H

#include <rpc/util/someip_util.h>

using qti::hal::rpc::SomeipMessage;

void SupplicantProcessSomeIPRequestMessage(const std::shared_ptr<SomeipMessage> &msg);

bool someip_send_message(std::shared_ptr<SomeipMessage> message);

#endif /* SUPPLICANT_MESSAGE_HANDLER_H */
