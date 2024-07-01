/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <thread>
#include <rpc/util/someip_common_def.h>
#include <rpc/message/hostapd/hostapd_message_def.h>
#include <rpc/util/log_common.h>
#include <rpc/util/someip_server.h>
#include "hostapd_someip_server.h"
#include "hostapd_message_handler.h"
#include "hostapd_event_callback.h"
#include "aidl_sock.h"

#define HOSTAPD_INSTANCE_ID      0x4440
#define HOSTAPD_EVENTGROUP_ID    0xDDD0
#define HOSTAPD_SERVICE_NAME     "hostapd_someip_service"

using qti::hal::rpc::SomeipContext;
using qti::hal::rpc::SomeipCallback;
using qti::hal::rpc::SomeipServer;

std::shared_ptr<std::thread> hostapd_someip_service_thread;
std::shared_ptr<SomeipServer> hostapd_someip_server;

bool HostapdSomeIPServerInit()
{
    /*Initialize Context*/
    SomeipContext context(WIFI_HOSTAPD_SERVICE_ID, HOSTAPD_INSTANCE_ID,
        HOSTAPD_EVENTGROUP_ID, HostapdEvent);
    SomeipCallback cb(nullptr,
                      nullptr,
                      HostapdProcessSomeIPRequestMessage);
    context.registerCallback(cb);
    hostapd_someip_server = std::make_shared<SomeipServer>(HOSTAPD_SERVICE_NAME, context);

    if (!hostapd_someip_server) {
        return false;
    }
    hostapd_someip_server->initMessageSchedule(hostapd_notify_aidl_socket);
    return hostapd_someip_server->init();
}

bool HostapdSomeIPServerStart()
{
    if(!hostapd_connect_aidl_socket())
        return false;
    ALOGI("Hostapd someip service loop starting...");
    hostapd_someip_service_thread =
        std::make_shared<std::thread>(&SomeipServer::start, hostapd_someip_server, false);
    return true;
}

void HostapdSomeIPServerStop()
{
    if (!hostapd_someip_server)
        return;
    ALOGI("Hostapd someip service main loop stop...");
    hostapd_someip_server->stop();
    hostapd_disconnect_aidl_socket();
    if(hostapd_someip_service_thread) {
        if (hostapd_someip_service_thread->joinable())
            hostapd_someip_service_thread->join();
    }
}

void HostapdSomeIPServerDeinit()
{
    if (!hostapd_someip_server)
        return;
    ALOGI("Hostapd someip service deinit...");
    hostapd_someip_server->deinit();
    hostapd_someip_server = nullptr;
}

void someip_process_queued_msg()
{
    if (!hostapd_someip_server)
        return;
    hostapd_someip_server->handleMessageQueue();
}

bool someip_send_request(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendRequest(message_type, data);
}

bool someip_send_response(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendResponse(message_type, data);
}

bool someip_send_event(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendEvent(message_type, data);
}

bool someip_send_message(std::shared_ptr<SomeipMessage> message)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendMessage(message);
}
