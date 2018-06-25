#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcasesTh.h"
#include "filehandler.h"
#include "Logger.h"

using namespace FuzzLogging;

thread_control::thread_control(canSocket * socket,int spi)
{
    _socket = socket;
    _spi = spi;
    _filter = new dynamicInputfilter();
    resetSignal();
}

thread_control::~thread_control()
{
    delete _filter;
}

void thread_control::resetSignal()
{
    _wait =true;
    _error = 0;
    _interrupt =false;
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
    if(_error != 0) return true;
    return false;
}

bool thread_control::isFilterStable(int ms)
{
    util::easytimer timer;
    util::easytimer testtime;
    bool stable = true;
    can_frame response;

    int i = 0;
    while(!_interrupt)
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
            LOG_INFO(util::stdCANframe(response), getChannelNameByNumber(getLogNumber()));
            stable = false;
        }
        if(testtime.XmsPassed(ms))
        {
            //decide
            if(stable)
            {
                LOG_INFO("filter-stable", getChannelNameByNumber(getLogNumber()));
                return true;
            }
            else
            {
                LOG_INFO("filter-unstable", getChannelNameByNumber(getLogNumber()));
                return false;
            }
        }
    }
    return 0;
}





int th_test::stoertest1Do(thread_control *ctl)
{
    LOG_INFO("log2Logger-stoertest", getChannelNameByNumber(1));
    //while(control->isWaiting()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    LOG_INFO("log2Logger-filter stable?", getChannelNameByNumber(1));

    //canSocket * spi0 = new canSocket(0);
    //thread_control * ctl = new thread_control(spi0,0);

    if(!ctl->isFilterStable(2000))
    {
        if(!ctl->isFilterStable(4000))
        {
            if(!ctl->isFilterStable(8000))
            {
                LOG_INFO("log2Logger-filter not stablelising", getChannelNameByNumber(1));
                ctl->errorFilterUnstable();
            }
        }
    }
    (!ctl->isFilterStable(10000));

    while(true);

    return 0;

}

int th_test::stoertest1()
{
    std::thread t[3];

    thread_control * ctl[3];

    canSocket * spi0 = new canSocket(0);
    canSocket * spi1 = new canSocket(1);
    canSocket * spi2 = new canSocket(2);

    ctl[0] = new thread_control(spi0,0);
    ctl[1] = new thread_control(spi1,1);
    ctl[2] = new thread_control(spi2,2);

    t[0] = std::thread(stoertest1Do,ctl[0]);
    t[1] = std::thread(stoertest1Do,ctl[1]);
    t[2] = std::thread(stoertest1Do,ctl[2]);

    std::cout << "Launched from the main\n";

    util::easytimer timer;

    int stop = -1;

    //overwatch threads 1 minute
    while (timer.XmsPassed(1000*60)) {
        //check once per second
        std::this_thread::sleep_for (std::chrono::milliseconds(1000));
        for(int i = 0; i < 3; i++)
        {
            if(stop!=-1)
            {
                break;
            }
            if(ctl[i]->hasError()) stop = i;
        }
    if(stop!=-1) break;
    }
    if(stop!=-1)
    {
        for(int i = 0; i < 3; i++)
        {
            ctl[i]->interrupt();
        }
    }
    //Join the threads with the main thread
    for (int i = 0; i < 3; ++i) {
        t[i].join();
    }
    return stop;
}






void th_test::log2LoggerDo(thread_control *control)
{
    LOG_INFO("log2Logger-waiting", getChannelNameByNumber(control->getLogNumber()));
    while(control->isWaiting()){ std::this_thread::sleep_for (std::chrono::milliseconds(1)); }

    LOG_INFO("log2Logger-listening", getChannelNameByNumber(control->getLogNumber()));

    int t = 0;
    std::ostringstream ss;
    while (true)
    {
        ss << " spi: " << control->getSPI() << " uptime:  " << t << "\n";
        LOG_INFO(ss, getChannelNameByNumber(control->getSPI()));
        ss.str("");
        ss.clear();
        sleep(1);
        if(control->hasInterrupt())
        {
            LOG_INFO("log2Logger-interupted", getChannelNameByNumber(control->getLogNumber()));
            break;
        }
        t++;
    }

    LOG_INFO("log2Logger-done", getChannelNameByNumber(control->getLogNumber()));

}

void th_test::log2Logger()
{
    std::thread t[3];

    thread_control * ctl[3];

    canSocket * spi0 = new canSocket(0);
    canSocket * spi1 = new canSocket(1);
    canSocket * spi2 = new canSocket(2);

    ctl[0] = new thread_control(spi0,0);
    ctl[1] = new thread_control(spi1,1);
    ctl[2] = new thread_control(spi2,2);

    t[0] = std::thread(log2LoggerDo,ctl[0]);
    t[1] = std::thread(log2LoggerDo,ctl[1]);
    t[2] = std::thread(log2LoggerDo,ctl[2]);

    std::cout << "Launched from the main\n";

    ctl[0]->start();
    sleep(5);
    ctl[1]->start();
    sleep(5);
    ctl[2]->start();
    sleep(10);
    ctl[0]->interrupt();
    sleep(10);
    ctl[1]->error();
    sleep(5);
    if(ctl[1]->hasError());
    ctl[1]->interrupt();
    ctl[2]->interrupt();

    //Join the threads with the main thread
    for (int i = 0; i < 3; ++i) {
        t[i].join();
    }
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


