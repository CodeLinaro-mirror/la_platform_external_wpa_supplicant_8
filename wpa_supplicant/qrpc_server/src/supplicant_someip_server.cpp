/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <thread>

#include <rpc/util/log_common.h>
#include <rpc/util/someip_server.h>

#include "supplicant_someip_server.h"
#include "supplicant_message_handler.h"
#include "supplicant_event_callback.h"

#include "aidl_sock.h"

#define SUPPLICANT_INSTANCE_ID                              ((uint16_t) 0x3330)
#define SUPPLICANT_EVENTGROUP_ID                            ((uint16_t) 0xCCC0)

using qti::hal::rpc::SomeipContext;
using qti::hal::rpc::SomeipCallback;
using qti::hal::rpc::SomeipServer;

std::string SUPPLICANT_SERVICE_NAME = "supplicant_someip_service";

std::shared_ptr<std::thread> wpas_someip_service_thread;
std::shared_ptr<SomeipServer> wpas_someip_server;

bool SupplicantSomeIPServerInit()
{
    /*Initialize Context*/
    SomeipContext context(WIFI_SUPPLICANT_SERVICE_ID, SUPPLICANT_INSTANCE_ID,
        SUPPLICANT_EVENTGROUP_ID, SupplicantEvent);

    SomeipCallback cb(nullptr,
                      nullptr,
                      SupplicantProcessSomeIPRequestMessage);
    context.registerCallback(cb);

    wpas_someip_server = std::make_shared<SomeipServer>(SUPPLICANT_SERVICE_NAME, context);

    if (!wpas_someip_server) {
        return false;
    }

    wpas_someip_server->initMessageSchedule(wpas_notify_aidl_socket);

    return wpas_someip_server->init();
}

bool SupplicantSomeIPServerStart()
{
    if(!wpas_connect_aidl_socket())
        return false;

    ALOGI("Supplicant someip service loop starting...");
    wpas_someip_service_thread =
        std::make_shared<std::thread>(&SomeipServer::start, wpas_someip_server, false);

    return true;
}

void SupplicantSomeIPServerStop()
{
    if (!wpas_someip_server)
        return;
    ALOGI("Supplicant someip service main loop stop...");
    wpas_someip_server->stop();

    wpas_disconnect_aidl_socket();

    if(wpas_someip_service_thread) {
        if (wpas_someip_service_thread->joinable())
            wpas_someip_service_thread->join();
    }
}

void SupplicantSomeIPServerDeinit()
{
    if (!wpas_someip_server)
        return;
    ALOGI("Supplicant someip service deinit...");
    wpas_someip_server->deinit();
    wpas_someip_server = nullptr;
}

void someip_process_queued_msg()
{
    if (!wpas_someip_server)
        return;
    wpas_someip_server->handleMessageQueue();
}

bool someip_send_request(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendRequest(message_type, data);
}

bool someip_send_response(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendResponse(message_type, data);
}

bool someip_send_event(uint16_t message_type, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendEvent(message_type, data);
}

bool someip_send_message(std::shared_ptr<SomeipMessage> message)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendMessage(message);
}
