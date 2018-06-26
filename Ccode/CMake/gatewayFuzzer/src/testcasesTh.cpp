#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcasesTh.h"
#include "filehandler.h"
#include "Logger.h"

using namespace FuzzLogging;

thread_control::thread_control(canSocket * socket, int spi, int id, int spiStoer)
{
    _socket = socket;
    _spi = spi;
    _filter = new dynamicInputfilter();
    _idStoer = id;
    _spiStoer = spiStoer;
    resetSignal();
}

thread_control::~thread_control()
{
    delete _filter;
}

void thread_control::resetSignal()
{
    _wait =true;
    _error = e_noerror;
    _interrupt =false;
    _done = false;
    _isStable = false;
}

void thread_control::errorDeadChannel()
{
    _error = e_deadchannel;
}

void thread_control::errorFilterUnstable()
{
    _error = e_filterunstable;
}

int thread_control::getError()
{
    return _error;
}

bool thread_control::hasError()
{
    if(_error != e_noerror) return true;
    return false;
}

bool thread_control::stopEarly()
{
    if(_error != e_noerror) return true;
    if(_interrupt) return true;
    if(_done) return true;
    return false;
}

void thread_control::filterBlockIdOnSender()
{
    if(_spi == _spiStoer) _filter->setblockby(_idStoer,allways);
}

bool thread_control::isAnythingNew(int ms, bool stopOnFirstNew)
{
    util::easytimer timer;
    util::easytimer testtime;
    bool stable = true;
    can_frame response;

    int i = 0;
    while(!stopEarly())
    {
        i++;
        if(_socket->canRecieveOne(&response,MSG_DONTWAIT)) timer.reset();
        else
        {
            if((i % 100) == 0){
                if(timer.XmsPassed(5000))
                {
                    LOG_ERROR("no msg from car in 5s",getChannelNameByNumber(getLogNumber()));
                    this->errorDeadChannel();
                }
            }
            //prevent busy waiting
            //top %CPU dropped from 95% to 0,7%
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            continue;
        }
        if(!_filter->testframe(&response))
        {
            LOG_MESSAGE(util::stdCANframe(response), getChannelNameByNumber(getLogNumber()));
            stable = false;
            if(stopOnFirstNew) return true;
        }
        if(testtime.XmsPassed(ms))
        {
            //decide
            if(stable)
            {
                LOG_INFO("nothing new", getChannelNameByNumber(getLogNumber()));
                return false;
            }
            else
            {
                LOG_INFO("new msg", getChannelNameByNumber(getLogNumber()));
                return true;
            }
        }
    }
    return false;
}

bool thread_control::isFilterStable(int ms)
{
    util::easytimer timer;
    util::easytimer testtime;
    bool stable = true;
    can_frame response;

    int i = 0;
    while(!stopEarly())
    {
        i++;
        if(_socket->canRecieveOne(&response,MSG_DONTWAIT)) timer.reset();
        else
        {
            if((i % 100) == 0){
                if(timer.XmsPassed(5000))
                {
                    LOG_ERROR("no msg from car in 5s",getChannelNameByNumber(getLogNumber()));
                    this->errorDeadChannel();
                }
            }
            //prevent busy waiting
            //top %CPU dropped from 95% to 0,7%
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            continue;
        }
        if(!_filter->testframe(&response))
        {
            LOG_MESSAGE(util::stdCANframe(response), getChannelNameByNumber(getLogNumber()));
            stable = false;
        }
        if(testtime.XmsPassed(ms))
        {
            //decide
            if(stable)
            {
                LOG_INFO("filter-stable", getChannelNameByNumber(getLogNumber()));
                _isStable  =true;
                return true;
            }
            else
            {
                LOG_INFO("filter-unstable", getChannelNameByNumber(getLogNumber()));
                return false;
            }
        }
    }
    return false;
}

bool thread_control::sendStoerung(int id, int number)
{
    int perDLC = number / 8;
    char data[8];
    for(int x=0;x<8;x++)data[x] = x;
    for(int i=0;i<8;i++)
    {
        for(int j = 0; j<perDLC; j++)
        {
            if(stopEarly()) return false;
            _socket->canSend(id,i+1,data,false);
            sleep(1);
        }
    }
    return true;
}


int th_test::stoertest1DoStoerung(thread_control *ctl)
{
    LOG_INFO("log2Logger-stoer thread waitign for stable", getChannelNameByNumber(ctl->getSPI()));
    while((ctl->isWaiting()) && !ctl->hasInterrupt()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }
    LOG_INFO("log2Logger-stoer thread start", getChannelNameByNumber(ctl->getSPI()));

    if(ctl->sendStoerung(ctl->getID(),200))
    {
        LOG_INFO("log2Logger sending stoped eary", getChannelNameByNumber(ctl->getSPI()));
    }
    else
    {
        LOG_INFO("log2Logger sending finished", getChannelNameByNumber(ctl->getSPI()));
    }

    ctl->done();

    return 0;

}

int th_test::stoertest1Do(thread_control *ctl)
{
    LOG_INFO("log2Logger-stoertest", getChannelNameByNumber(ctl->getSPI()));
    //while(control->isWaiting()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    LOG_INFO("log2Logger-filter stable?", getChannelNameByNumber(ctl->getSPI()));

    // ToDo set filter default
    ctl->_filter->setStaticBlockList(ctl->getSPI());

    //test Filter
    if(!ctl->isFilterStable(2000))
    {
        if(ctl->stopEarly()) goto stop;
        if(!ctl->isFilterStable(4000))
        {
            if(ctl->stopEarly()) goto stop;
            if(!ctl->isFilterStable(8000))
            {
                LOG_INFO("log2Logger-filter not stablelising", getChannelNameByNumber(ctl->getSPI()));
                ctl->errorFilterUnstable();
                if(ctl->stopEarly()) goto stop;
            }
        }
    }
    //List everything that goes through the filter

    if(ctl->isAnythingNew(20000,true))
    {
        LOG_INFO("log2Logger new msg detected", getChannelNameByNumber(ctl->getSPI()));
    }
    else
    {
        LOG_INFO("log2Logger nothing", getChannelNameByNumber(ctl->getSPI()));
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

int th_test::stoertest1(int id, int spiStoerung)
{
    int tc = 4;
    std::thread t[tc];
    thread_control * ctl[tc];

    canSocket * spi0 = new canSocket(0);
    canSocket * spi1 = new canSocket(1);
    canSocket * spi2 = new canSocket(2);
    canSocket * spiSend = new canSocket(spiStoerung);

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
            if(stable == 3) ctl[3]->start();
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
        if(done >= tc)
        {
            LOG_ERROR("log2Logger-all threads finished", FuzzLogging::debugfile);
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


    return stop;
}


void th_test::testLOG(canSocket * Socket, int spi)
{
    Dmesg * dm = Dmesg::getInstance();

    std::string name = dm->nameSPI(spi);

    std::ostringstream ss;

    for(int t = 0; t < 30; t++)
    {
        ss << name << " uptime:  " << t << " spi: " << spi << "\n";
        LOG_INFO(ss, getChannelNameByNumber(spi));
        ss.str("");
        ss.clear();
        sleep(1);
    }

    ss << name << " done  " << "\n";
    LOG_INFO(ss, getChannelNameByNumber(spi));

}

void th_test::threadedTest()
{
    std::thread t[3];

    //Launch a group of threads
    canSocket * socket0 = new canSocket(0);
    canSocket * socket1 = new canSocket(1);
    canSocket * socket2 = new canSocket(2);
    t[0] = std::thread(testLOG,socket0,0);
    t[1] = std::thread(testLOG,socket1,1);
    t[2] = std::thread(testLOG,socket2,2);

    std::cout << "Launched from the main\n";

    //Join the threads with the main thread
    for (int i = 0; i < 3; ++i) {
        t[i].join();
    }


}


