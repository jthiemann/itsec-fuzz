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
enum errors {e_noerror,e_deadchannel, e_filterunstable, e_unknown};

namespace util {


int printCANframe(can_frame frame, const char *iface);
std::string stdCANframe(can_frame frame);

void deepcopyCANframe(can_frame src, can_frame dest);

std::string toHexString(int hex);

//spi-CANx problem
int updateDMESG();

//relay wird per Bashskript gesteuert das hier aufgerufen wird
//Vorteil wiringPI muss nicht mitcompiliert werden
int setupPIN();
int setPIN(bool state);



class easytimer
{
public:
    easytimer();
    int timePassedms();
    void reset();
    bool XmsPassed(int ms);

private:
    struct timeval lasttime, temp;

};

}

