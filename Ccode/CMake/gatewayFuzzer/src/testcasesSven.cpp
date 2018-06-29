#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcasesTh.h"
#include "testcasesSven.h"
#include "filehandler.h"
#include "Logger.h"

using namespace FuzzLogging;


int th_test_sven::stoertest1DoStoerung(thread_control *ctl)
{
    LOG_INFO("stoerthread waiting", FuzzLogging::debugfile);
    while((ctl->isWaiting()) && !ctl->hasInterrupt()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    util::easytimer wait30;

    while(!wait30.XmsPassed(30*1000) && !ctl->hasInterrupt()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    LOG_INFO("log2Logger-stoer thread start", FuzzLogging::debugfile);

    if(ctl->sendStoerung(ctl->getID(),500,60))
    //if(ctl->generateCANMessages(ctl->getID(),10,1000))
    {
        LOG_INFO("sending finished", FuzzLogging::debugfile);
    }
    else
    {
        LOG_INFO("sending stoped early", FuzzLogging::debugfile);
    }

    ctl->done();

    return 0;

}

int th_test_sven::stoertest1Do(thread_control *ctl)
{
    std::string idstr = "log spi";
    idstr+= std::to_string(ctl->getSPI());
    idstr+= " stoerID: ";
    idstr+= util::toHexString(ctl->getID());
    LOG_INFO(idstr,getChannelNameByNumber(ctl->getSPI()));

    // ToDo set filter default
    ctl->_filter->setStaticBlockList(ctl->getSPI());
    ctl->filterBlockIdOnSender();
    //test Filter
    if(!ctl->isFilterStable(2000))
    {
        if(ctl->stopEarly()) goto stop;
        if(!ctl->isFilterStable(4000))
        {
            if(ctl->stopEarly()) goto stop;
            if(!ctl->isFilterStable(8000))
            {
                ctl->errorFilterUnstable();
                if(ctl->stopEarly()) goto stop;
            }
        }
    }
    //List everything that goes through the filter

    while((ctl->isWaiting()) && !ctl->hasInterrupt()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    if(ctl->simpleLog(30*1000))  //30s ohne störung
    {
        LOG_INFO("reference Log done", getChannelNameByNumber(ctl->getSPI()));
    }
    else
    {
        LOG_INFO("error while reference Log", getChannelNameByNumber(ctl->getSPI()));
        goto stop;
    }

    if(ctl->simpleLog(30*1000))  //30s mit störung
    {
        LOG_INFO("stoer Log done", getChannelNameByNumber(ctl->getSPI()));
    }
    else
    {
        LOG_INFO("error while stoer Log", getChannelNameByNumber(ctl->getSPI()));
        goto stop;
    }

    ctl->done();
    LOG_INFO("log2Logger done", getChannelNameByNumber(ctl->getSPI()));


    return 0;

stop:

    if(ctl->hasError())
    {
        if(ctl->getError() == e_deadchannel) LOG_ERROR("log2Logger-dead Interface", getChannelNameByNumber(ctl->getSPI()));
        if(ctl->getError() == e_filterunstable) LOG_ERROR("log2Logger-filter not stablelising", getChannelNameByNumber(ctl->getSPI()));
        if(ctl->getError() == e_unknown) LOG_ERROR("log2Logger-unknown error", getChannelNameByNumber(ctl->getSPI()));
    }
    if(ctl->hasInterrupt()) LOG_INFO("log2Logger was interrupted", getChannelNameByNumber(ctl->getSPI()));

    return 0;

}

int th_test_sven::stoertest1(int id, int spiStoerung, canSocket * spi0,canSocket * spi1,canSocket * spi2,canSocket * spiSend)
{
    int tc = 4;
    std::thread t[tc];
    thread_control * ctl[tc];

    ctl[0] = new thread_control(spi0,0,id,spiStoerung);
    ctl[1] = new thread_control(spi1,1,id,spiStoerung);
    ctl[2] = new thread_control(spi2,2,id,spiStoerung);

    ctl[3] = new thread_control(spiSend,2,id,spiStoerung);

    t[0] = std::thread(stoertest1Do,ctl[0]);
    //sleep(5);
    t[1] = std::thread(stoertest1Do,ctl[1]);
    t[2] = std::thread(stoertest1Do,ctl[2]);

    t[3] = std::thread(stoertest1DoStoerung,ctl[3]);

    util::easytimer timer;

    int stop = -1;

    //overwatch threads 1 minute
    while (!timer.XmsPassed(1000*60))
    {
        int done = 0;
        int stable = 0;
        for(int i = 0; i < tc; i++)
        {
            if(stop!=-1)
            {
                break;
            }
            if(ctl[i]->isStable()) stable ++;
            if(stable == 3)
            {
                ctl[0]->start();
                ctl[1]->start();
                ctl[2]->start();
                ctl[3]->start();
            }
            if(ctl[i]->stopEarly())
            {
                if(ctl[i]->hasError())
                {
                    stop = i;
                    LOG_ERROR("log2Logger-error in thread", FuzzLogging::debugfile);
                }
                if(ctl[i]->isDone())
                {
                    done++;
                }

            }
        }
        //finish all listeners after stoerthread
        if(ctl[3]->isDone()) for(int j = 0 ; j < tc; j++) ctl[j]->interrupt();
        if(done >= (tc -1))
        {
            for(int j = 0 ; j < tc; j++) ctl[j]->interrupt();
        }
        if(done >= tc)
        {
            LOG_INFO("log2Logger-all threads finished", FuzzLogging::debugfile);
            break;
        }
        if(stop!=-1) break;
        //check once per second
        std::this_thread::sleep_for (std::chrono::milliseconds(1000));
    }
    //Stop all other threads after error
    if(stop!=-1)
    {
        for(int i = 0; i < tc; i++)
        {
            LOG_ERROR("log2Logger-interupting", FuzzLogging::debugfile);
            ctl[i]->interrupt();
        }
    }
    //Join the threads with the main thread
    for (int i = 0; i < tc; ++i) {
        t[i].join();
    }
    //Restart interface and Car if needed

    Dmesg *dm = Dmesg::getInstance();


    if(ctl[stop]->getError() == e_deadchannel)
    {
        dm->downSPIAll();
        sleep(2);
        dm->upSPIAll();
    }

    if(ctl[stop]->getError() == e_filterunstable)
    {
        util::setupPIN();
        util::setPIN(true);
        sleep(3); //sekunden pause power off
        util::setPIN(false);
        sleep(40); //booting car
    }

    if((stop != -1) && ctl[stop]->hasError())
    {
        std::string out = "spi"+std::to_string(spiStoerung)+" <- 0x"+util::toHexString(id)+ " | ";
        for (int i = 0; i < 3 ; ++i) out += std::to_string(ctl[i]->getDetected()) + " : ";
        out += "had error";
        LOG_INFO(out,FuzzLogging::debugfile);
    }
    else
    {
        std::string out = "spi"+std::to_string(spiStoerung)+" <- 0x"+util::toHexString(id)+ " | ";
        for (int i = 0; i < 3 ; ++i) out += std::to_string(ctl[i]->getDetected()) + " : ";
        LOG_INFO(out,FuzzLogging::resultfile);
    }

    for (int i = 0; i < tc ; ++i) delete ctl[i];

    if(stop != -1) return -1;
    return 0;
}

void th_test_sven::stoertest1Loop(int startID, int stopID,int spiStoerung)
{
    canSocket * spi0 = new canSocket(0);
    canSocket * spi1 = new canSocket(1);
    canSocket * spi2 = new canSocket(2);
    canSocket * spiSend = new canSocket(spiStoerung);

    for(int i = startID; i <= stopID; i++)
    {
        if(stoertest1(i,spiStoerung,spi0,spi1,spi2,spiSend) != 0) i--;
    }

    delete spi0;
    delete spi1;
    delete spi2;
    delete spiSend;
}






