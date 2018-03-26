#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <thread>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <sys/time.h>

#pragma once

enum blockby {once,allways,list,cyclic,update};

namespace util {

int printCANframe(can_frame frame, const char *iface);
void deepcopyCANframe(can_frame src, can_frame dest);

}

