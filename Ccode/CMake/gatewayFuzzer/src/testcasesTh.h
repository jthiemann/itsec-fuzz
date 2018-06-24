
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include <thread>
#include <mutex>
#include "canSocket.h"

class thread_control
{
public:

    thread_control(int spi);
    ~thread_control();
    void resetSignal();
    void start() {_wait = false;}
    void interrupt(){_interrupt = true;}
    void error() {_error = true;}

    canSocket * getSocket() { return _socket; }
    int getSPI() { return _spi;}
    int getLogNumber() { return _spi;}
    bool isWaiting() { return _wait;}
    bool hasError() { return _error;}
    bool hasInterrupt() {return _interrupt;}



private:
    int _spi;
    bool _interrupt;
    bool _wait;
    bool _error;
    canSocket * _socket;




};

namespace th_test {

void threadedTest();
void testLOG(canSocket * Socket, int spi);
void log2Logger();
void log2LoggerDo(thread_control * control);

}
