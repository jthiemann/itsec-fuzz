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
#include "testcaseCyclic.h"
#include "filehandler.h"
#include "testcasesTh.h"
#include "testcasesSven.h"
#include "Logger.h"

int main(void)
{
        FuzzLogging::LOG_INFO("=======================NEW PROGRAM START!!!=======================", FuzzLogging::debugfile);
        Dmesg * dm = Dmesg::getInstance();
        dm->readDmesg();
        //dm->upSPIAll();
        //dm->downSPIAll();
        //test::config(dm->nameSPI0_0().c_str(),dm->nameSPI0_1().c_str(),dm->nameSPI1_0().c_str());
        //dm->setVCAN(true);
        test::config(dm->nameSPI0_0().c_str(),dm->nameSPI0_1().c_str(),dm->nameSPI1_0().c_str());

        //test::detectCanDown(conf->NameComfort());

        //th_test::threadedTest();
        //th_test::log2Logger();

        //FuzzLogging::LOG_INFO("usleep", FuzzLogging::debugfile);
        //for(int i = 1; i <=10; i++)
        //{
        //    FuzzLogging::LOG_INFO(std::to_string(i), FuzzLogging::debugfile);
        //    usleep(1000*1000);
        //}

        //th_test::stoertest1Loop(0x200,0x7FF,2);
        th_test_sven::stoertest1Loop(0x200,0x7FF,2);


        //util::setupPIN();
        //util::setPIN(true);
        //sleep(5);
        //util::setPIN(false);

        //test::Sending("vcan0");
        //test::SendRecievePair("vcan0","vcan1");
        //test::Wait4SpezialMsgID(0x01C,"vcan1",50000);
        //test::SendAndWait4AllMsgID("can0","can1");
        //test::dynamicInputfilterTest(conf->NameComfort());
        //test::dynamicInputfilterTest(dm->nameSPI1_0().c_str());

        //util::updateDMESG();

        //test::findCyclesLog(dm->nameSPI0_0().c_str(),dm->nameSPI0_1().c_str(),dm->nameSPI1_0().c_str(),true);
}
