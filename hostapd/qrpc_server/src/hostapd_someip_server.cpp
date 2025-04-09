/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <thread>
#include <rpc/util/someip_common_def.h>
#include <rpc/util/properties.h>
#include <rpc/message/hostapd/hostapd_message_def.h>
#include <rpc/util/log_common.h>
#include <rpc/util/someip_server.h>
#include <rpc/util/aidl_sock.h>
#include <rpc/util/message_queue.h>
#include "hostapd_someip_server.h"
#include "hostapd_message_handler.h"
#include "hostapd_event_callback.h"

#define HOSTAPD_INSTANCE_ID_CHM      ((uint16_t) 0x4440)
#define HOSTAPD_INSTANCE_ID_CEM      ((uint16_t) 0x4441)
#define HOSTAPD_EVENTGROUP_ID        ((uint16_t) 0xDDD0)

using qti::hal::rpc::SomeipContext;
using qti::hal::rpc::SomeipCallback;
using qti::hal::rpc::SomeipServer;
using qti::hal::rpc::SomeipMessage;
using qti::hal::rpc::Someip;
using qti::hal::rpc::SomeipMessageHandler;

std::string HOSTAPD_SERVICE_NAME = "hostapd_someip_service";
std::shared_ptr<std::thread> hostapd_someip_service_thread;
std::shared_ptr<SomeipServer> hostapd_someip_server;

std::string cpath = "/data/vendor/wifi/hostapd/aidl_client";
std::shared_ptr<MessageQueue<std::shared_ptr<SomeipMessage>>> hostapd_someip_msg_queue;

static void hostapd_msg_schedule(const std::shared_ptr<SomeipMessage> &msg)
{
    hostapd_someip_msg_queue->push(msg);
    qti_notify_aidl_socket();
}

static void someip_process_queued_msg()
{
    if (!hostapd_someip_server)
        return;
    if(hostapd_someip_msg_queue->empty()){
        ALOGE("Error: Message queue is empty");
        return;
    }
    auto message = hostapd_someip_msg_queue->front();
    hostapd_someip_msg_queue->pop();
    HostapdProcessSomeIPRequestMessage(message);
    message.reset();
}


bool HostapdSomeIPServerInit()
{
    uint16_t hostapd_instance_id;
    char someip_config_file[PROPERTY_VALUE_MAX];
    property_get("persist.vendor.someip.config_file", someip_config_file, "/etc/someip/vsomeip_server.json");
    if(strstr(someip_config_file, "cem")){
        hostapd_instance_id = HOSTAPD_INSTANCE_ID_CEM;
        ALOGI("Hostapd CHM someip service init...");
    }else{
        hostapd_instance_id = HOSTAPD_INSTANCE_ID_CHM;
        ALOGI("Hostapd CEM someip service init...");
    }

    /*Initialize Context*/
    SomeipContext context(WIFI_HOSTAPD_SERVICE_ID, hostapd_instance_id,
        HOSTAPD_EVENTGROUP_ID, HostapdEventArray);
    SomeipCallback cb(nullptr,
                      hostapd_msg_schedule);
    Someip::setup(cb);

    hostapd_someip_server = std::make_shared<SomeipServer>(HOSTAPD_SERVICE_NAME, context);

    if (!hostapd_someip_server) {
        return false;
    }
    hostapd_someip_msg_queue = std::make_shared<MessageQueue<std::shared_ptr<SomeipMessage>>>();
    ALOGI("Hostapd someip service is initialized");
    return true;
}

bool HostapdSomeIPServerStart()
{
    if(!qti_connect_aidl_socket(cpath.c_str(), someip_process_queued_msg))
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
    qti_disconnect_aidl_socket();
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
    hostapd_someip_msg_queue = nullptr;
    hostapd_someip_server = nullptr;
}

bool someip_send_event(uint16_t method_id, std::vector<uint8_t> &data)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendEvent(method_id, data);
}

bool someip_send_message(std::shared_ptr<SomeipMessage> message)
{
    if (!hostapd_someip_server)
        return false;
    return hostapd_someip_server->sendMessage(message);
}
