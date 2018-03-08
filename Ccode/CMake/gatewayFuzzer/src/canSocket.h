#include "util.h"

class canSocket
{
public:
    canSocket(const char * ifname);
    bool canSend(int id, int length, char * data, bool extended);
    bool canSendBatch();

    bool canRecieveWait4ExpectedMSG(can_frame input_frame, can_frame * outputframe);
    bool canRecieveOne(can_frame * frame, int flags);

private:

    int s;
    int nbytes;
    int error = 0;
    struct sockaddr_can addr;
    struct can_frame recv_frame;
    struct can_frame send_frame;
    struct ifreq ifr;
    const char *_ifname;
};

