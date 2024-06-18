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
#include "SupplicantNonStdCallback.h"

#include "aidl_sock.h"

#define SUPPLICANT_INSTANCE_ID                              ((uint16_t) 0x3330)
#define SUPPLICANT_EVENTGROUP_ID                            ((uint16_t) 0xCCC0)

using qti::hal::rpc::SomeipContext;
using qti::hal::rpc::SomeipCallback;
using qti::hal::rpc::SomeipServer;
using qti::hal::rpc::SomeipMessage;

std::string SUPPLICANT_SERVICE_NAME = "supplicant_someip_service";

std::shared_ptr<std::thread> wpas_someip_service_thread;
std::shared_ptr<SomeipServer> wpas_someip_server;

uint16_t reqId = 0xFFFF;
uint32_t defaultTimeout = 500;

std::shared_ptr<SomeipMessage> reqMsg;
std::condition_variable condition_;
std::mutex mutex_;

static std::shared_ptr<SomeipMessage> WaitforMsg(uint32_t timeout)
{
    ALOGD("Awaiting request (0x%04X) with timeout (%d)", reqId, defaultTimeout);

    std::shared_ptr<SomeipMessage> req;
    std::chrono::steady_clock::time_point elapsed(std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout));
    std::unique_lock<std::mutex> lock(mutex_);

    if (condition_.wait_until(lock, elapsed) == std::cv_status::timeout)
    {
        ALOGE("Timeout - Did not receive request (0x%04X)", reqId);
        return nullptr;
    }
    if (reqMsg->getMethodId() != reqId)
    {
        ALOGE("Received mismatched request (0x%04X)", reqMsg->getMethodId());
        return nullptr;
    }

    ALOGD("Received request (0x%04X)", reqId);

    req = reqMsg;
    reqMsg = nullptr;
    reqId = 0xFFFF;

    return req;
}

void wpas_msg_scheduler(const std::shared_ptr<SomeipMessage> &msg)
{
    if (msg->getMethodId() == reqId)
    {
        ALOGI("Received unlock request (0x%04X)", reqId);
        reqMsg = msg;
        condition_.notify_one();
    }

    wpas_notify_aidl_socket();
}


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

    if (!wpas_someip_server)
        return false;

    wpas_someip_server->initMessageSchedule(wpas_msg_scheduler);

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

    if(wpas_someip_service_thread)
    {
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

bool someip_send_request(uint16_t method_id, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendRequest(method_id, data);
}

bool someip_send_response(uint16_t method_id, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendResponse(method_id, data);
}

bool someip_send_event(uint16_t method_id, std::vector<uint8_t> &data)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendEvent(method_id, data);
}

bool someip_send_message(std::shared_ptr<SomeipMessage> message)
{
    if (!wpas_someip_server)
        return false;
    return wpas_someip_server->sendMessage(message);
}

std::shared_ptr<SomeipMessage> someip_send_nonstd_event(uint16_t method_id, const std::vector<uint8_t>& data, uint16_t unlock_id)
{
    if (!wpas_someip_server)
        return nullptr;
    if(!wpas_someip_server->sendEvent(method_id, data))
        return nullptr;

    ALOGI("Event (0x%04X) sent, awaiting request (0x%04X)", method_id, unlock_id);

    reqId = unlock_id;

    return WaitforMsg(defaultTimeout);
}
