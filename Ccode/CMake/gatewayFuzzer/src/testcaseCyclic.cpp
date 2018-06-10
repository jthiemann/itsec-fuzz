#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcaseCyclic.h"
#include "filehandler.h"

int test::findCyclesLog(const char * ifaceCom,const char * ifaceDia,const char * ifaceEng, bool reportOnly)
{
    for(int i = 0; i < 3; i++)
    {
        bool settled = false;
        char iface_reciever[30];
        if(i == 0 ) strcpy(iface_reciever,ifaceCom);
        if(i == 1 ) strcpy(iface_reciever,ifaceDia);
        if(i == 2 ) strcpy(iface_reciever,ifaceEng);

        int TimeFilterSettelms = 20 * 1000; //20 Sekunden
        int TimeFilterLogCycle = 21 * 1000;

        std::cout<<"\n-----\ndynamicInputfilterTest(const char * iface_reciever)\n";
        std::cout<<"filtered input"<< " to " << iface_reciever<<"\n";
        canSocket * reciever = new canSocket(iface_reciever);

        can_frame response;

        dynamicInputfilter * dynfilter = new dynamicInputfilter();


        std::cout<<"filter running for 20s....\n";

        struct timeval start, temp;
        gettimeofday(&start, 0);

        while(true)
        {
            gettimeofday(&temp, 0);
            int time = ((temp.tv_usec - start.tv_usec)/1000)+((temp.tv_sec - start.tv_sec)*1000);

            if((time > TimeFilterSettelms) && !settled)
            {
                std::cout<<"\nFilterSettled?\n..................................................\n";
                settled = true;
            }
            if((time > TimeFilterLogCycle))
            {
                std::cout<<"\nlogging Cyles done\n..................................................\n";
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
        std::cout<<dynfilter->getReportRList();

        dynfilter->~dynamicInputfilter();
        //delete reciever;
    }

}


