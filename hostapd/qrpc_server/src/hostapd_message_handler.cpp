/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <map>
#include <functional>
#include <rpc/util/common_util.h>
#include <rpc/util/someip_util.h>
#include <rpc/util/log_common.h>
#include <rpc/message/hostapd/hostapd_message_def.h>
#include <rpc/message/hostapd_vendor/hostapd_vendor_message_def.h>

#include "hostapd_message_handler.h"
#include "HostapdReq.h"
#include "HostapdVendorReq.h"


typedef std::function< bool (uint8_t* data, size_t length, std::vector<uint8_t>& outData) > MessageHandler;
static std::map<uint16_t, MessageHandler> msgHandlerMap = {
    /* IHostapd */
    {HOSTAPD_ADD_ACCESS_POINT_REQ,&HostapdMsgHandlerAddAccessPoint},
    {HOSTAPD_FORCE_CLIENT_DISCONNECT_REQ,&HostapdMsgHandlerForceClientDisconnect},
    {HOSTAPD_REMOVE_ACCESS_POINT_REQ,&HostapdMsgHandlerRemoveAccessPoint},
    {HOSTAPD_SET_DEBUG_PARAMS_REQ, &HostapdMsgHandlerSetDebugParams},
    {HOSTAPD_TERMINATE_REQ, &HostapdMsgHandlerTerminate},
    /* IHostapdVendor */
    {HOSTAPD_VENDOR_LIST_VENDOR_INTERFACES_REQ, &HostapdVendorMsgHandlerListVendorInterfaces},
    {HOSTAPD_VENDOR_DO_DRIVER_CMD_REQ, &HostapdVendorMsgHandlerDoDriverCmd},
};

static inline MessageHandler HostapdGetMessageHandler(uint16_t methodId)
{
    auto item = msgHandlerMap.find(methodId);
    if (item != msgHandlerMap.end())
        return item->second;
    return nullptr;
}

void HostapdProcessSomeIPRequestMessage(const std::shared_ptr<SomeipMessage> &msg)
{
    uint16_t methodId = msg->getMethodId();

    ALOGI("Recv Someip Request message with method_id 0x%04X", methodId);

    MessageHandler handler = HostapdGetMessageHandler(methodId);
    if (!handler) {
        ALOGE("Unsupported SomeIP request method id 0x%04X", methodId);
        return;
    }

    std::vector<uint8_t> response_data;
    bool ret = handler(msg->getData(), msg->getLength(), response_data);
    if (!ret) {
        ALOGE("Process SomeIP Request fail");
        return;
    }

    std::shared_ptr<SomeipMessage> response = std::make_shared<SomeipMessage>(msg->createResponse(response_data));
    ret = someip_send_message(response);
    if (!ret)
        ALOGE("Send SomeIP Response fail");
}
