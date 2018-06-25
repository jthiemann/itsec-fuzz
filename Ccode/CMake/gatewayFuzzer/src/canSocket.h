#include "util.h"
//#include "filehandler.h"

#pragma once

class canSocket
{
public:

    canSocket(int spi);
    canSocket(const char * ifname);
    bool canSend(int id, int length, char * data, bool extended);
    bool canSendBatch();

    bool canRecieveWait4ExpectedMSG(can_frame input_frame, can_frame * outputframe);
    bool canRecieveOne(can_frame * frame, int flags);

    int getFrequency() {return freq;}

private:

    int s;
    int nbytes;
    int error = 0;
    struct sockaddr_can addr;
    struct can_frame recv_frame;
    struct can_frame send_frame;
    struct ifreq ifr;
    const char *_ifname;
    int _spiNumber;
    int freq;
    struct timeval last;

};

