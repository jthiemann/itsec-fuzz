#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcaseCyclic.h"
#include "filehandler.h"
#include "Logger.h"

using namespace FuzzLogging;

int test::findCyclesLog(const char * ifaceCom,const char * ifaceDia,const char * ifaceEng, bool reportOnly)
{
    for(int i = 0; i < 3; i++)
    {
        bool settled = false;
        char iface_reciever[30];
        if(i == 0 ) {
            strcpy(iface_reciever,ifaceCom);
        }
        if(i == 1 ) {
            strcpy(iface_reciever,ifaceDia);
        }
        if(i == 2 ) {
            strcpy(iface_reciever,ifaceEng);
        }

        int TimeFilterSettelms = 20 * 1000; //20 Sekunden
        int TimeFilterLogCycle = 21 * 1000;

        LOG_INFO("DynamicInputfilterTest(const char * iface_reciever)", getChannelNameByNumber(i));

        std::ostringstream ss;
        ss << "filtered input to " << iface_reciever << std::endl;
        LOG_INFO(ss, getChannelNameByNumber(i));

        canSocket * reciever = new canSocket(iface_reciever);

        can_frame response;

        dynamicInputfilter * dynfilter = new dynamicInputfilter();

        LOG_INFO("filter running for 20s....", getChannelNameByNumber(i));

        struct timeval start, temp;
        gettimeofday(&start, 0);

        while(true)
        {
            gettimeofday(&temp, 0);
            int time = ((temp.tv_usec - start.tv_usec)/1000)+((temp.tv_sec - start.tv_sec)*1000);

            if((time > TimeFilterSettelms) && !settled)
            {
                LOG_INFO("FilterSettled?", getChannelNameByNumber(i));
                settled = true;
            }
            if((time > TimeFilterLogCycle))
            {
                LOG_INFO("logging Cyles done", getChannelNameByNumber(i));
                break;
            }


            if(reciever->canRecieveOne(&response,MSG_DONTWAIT));
            else
            {
                //prevent busy waiting
                //top %CPU dropped from 95% to 0,7%
                std::this_thread::sleep_for (std::chrono::milliseconds(1));
                continue;
            }

            if(!dynfilter->testframe(&response))
            {
                if(!reportOnly)util::printCANframe(response,iface_reciever);
            }

        }
        std::flush(std::cout);
        LOG_INFO(dynfilter->getReportRList(), getChannelNameByNumber(i));

        dynfilter->~dynamicInputfilter();
        //delete reciever;
    }

}


