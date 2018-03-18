#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/time.h>


#include "filter.h"
#include "canSocket.h"
#include "testcases.h"
#include "filehandler.h"

int main(void)
{
        Config * conf = new Config("../config.gateway");

        test::config(conf->NameComfort(),conf->NameDiagnose(),conf->NameEngine());

        test::Sending("vcan0");
        test::SendRecievePair("vcan0","vcan1");
        //test::Wait4SpezialMsgID(0x01C,"vcan1",50000);
        //test::SendAndWait4AllMsgID("can0","can1");
        test::dynamicInputfilterTest("vcan1");
}
