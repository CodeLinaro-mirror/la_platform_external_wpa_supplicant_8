/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include <iostream>
#include <thread>

#include <rpc/util/someip_api.h>
#include <rpc/util/someip_common_def.h>
#include <rpc/util/log_common.h>
#include <rpc/message/wpa_supplicant/supplicant_message_def.h>

#include "supplicant_someip_server.h"
#include "supplicant_message_handler.h"
#include "supplicant_event_callback.h"

static char SUPPLICANT_SERVICE_NAME[] = "supplicant_someip_service";

std::shared_ptr<std::thread> someip_server_thread_t;

#define SUPPLICANT_INSTANCE_ID                              ((uint16_t) 0x3330)
#define SUPPLICANT_EVENTGROUP_ID                            ((uint16_t) 0xCCC0)

static void SupplicantInitSomeIPRegisterInfo(SomeipRegisterInfo* info)
{
    info->context.app_name = SUPPLICANT_SERVICE_NAME;
    info->context.service_id = WIFI_SUPPLICANT_SERVICE_ID;
    info->context.service_instance_id = SUPPLICANT_INSTANCE_ID;

    info->context.eventgroup_id = SUPPLICANT_EVENTGROUP_ID;
    info->context.event_id_number = SUPPLICANT_EVENT_COUNTS;
    info->context.event_id = SupplicantEventArray;

    info->dataCallback = &SupplicantProcessSomeIPRequestMessage; 
}

bool SupplicantSomeIPServerInit()
{
    SomeipRegisterInfo registerInfo;
    SupplicantInitSomeIPRegisterInfo(&registerInfo);

    ALOGI("Supplicant someip service init...");

    uint8_t retryCount = 0;
    while (true) {
        retryCount++;
    if (someip_init(&registerInfo)) {
            ALOGI("Supplicant someip service app init successful");
            break;
        }

        if (retryCount > 20) {
            ALOGE("Supplicant Someip service init fail");
            return -1;
        }

        usleep(1000);
    }

    /* Start SomeIP Main Loop */
    someip_server_thread_t  = std::make_shared<std::thread>(
                                &SupplicantSomeIPServerStart);

    return true;
}

void SupplicantSomeIPServerDeinit()
{
    ALOGI("Supplicant someip service deinit...");
    someip_deinit();
}

bool SupplicantSomeIPServerStart()
{
    ALOGI("Supplicant someip service main loop start...");
    return someip_open();
}

void SupplicantSomeIPServerStop()
{
    ALOGI("Supplicant someip service main loop stop...");
    someip_close();
    if(someip_server_thread_t) {
        if (someip_server_thread_t->joinable())
            someip_server_thread_t->join();
    }
}
