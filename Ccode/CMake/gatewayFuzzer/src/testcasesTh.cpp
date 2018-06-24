#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcasesTh.h"
#include "filehandler.h"
#include "Logger.h"

thread_control::thread_control(int spi)
{
    _socket = new canSocket(spi);
    _spi = spi;
    resetSignal();
}

thread_control::~thread_control()
{
    delete _socket;
}

void thread_control::resetSignal()
{
    _wait =true;
    _error = false;
    _interrupt =false;
}

using namespace FuzzLogging;

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
    ctl[0] = new thread_control(0);
    ctl[1] = new thread_control(1);
    ctl[2] = new thread_control(2);

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


