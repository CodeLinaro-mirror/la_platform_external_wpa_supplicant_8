/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#ifndef ICINTERFACE_H
#define ICINTERFACE_H

namespace ndk {

class ICInterface {
public:
  ICInterface* get() { return this; }
};

}
#endif
