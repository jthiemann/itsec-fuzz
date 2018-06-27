
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include <thread>
#include <mutex>
#include "canSocket.h"
#include "filter.h"

class thread_control
{
public:

    thread_control(canSocket *socket, int spi, int id, int spiStoer);
    ~thread_control();
    void resetSignal();
    void start() {_wait = false;}
    void interrupt(){_interrupt = true;}
    void error() {_error = e_unknown;}
    void done() {_done= true;}
    void errorDeadChannel();
    void errorFilterUnstable();
    bool stopEarly();
    bool isStable() {return _isStable;}
    int getID() {return _idStoer;}
    int getSpiStoer() {return _spiStoer;}
    int getDetected() {return _detected;}

    canSocket * getSocket() { return _socket; }
    int getSPI() { return _spi;}
    int getLogNumber() { return _spi;}
    bool isWaiting() { return _wait;}
    bool isDone() { return _done;}
    bool hasError();
    int getError();
    bool hasInterrupt() {return _interrupt;}

    bool isFilterStable(int ms);
    bool isAnythingNew(int ms, bool stopOnFirstNew);

    bool sendStoerung(int id, int number, int timeout_ms);
    bool generateCANMessages(int id, int amount, int timeout_ms);
    void sendWaintingGap(int ms) {_time = ms;}

    void filterBlockIdOnSender();

    dynamicInputfilter * _filter;

private:

    bool _isStable;
    int _time;
    int _spi;
    bool _interrupt;
    bool _wait;
    bool _done;
    int _error;
    int _detected;
    canSocket * _socket;

// informationen über den test

    int _idStoer;
    int _spiStoer;






};



namespace th_test {

void threadedTest();
void testLOG(canSocket * Socket, int spi);
//jene Namen
void log2Logger();
void log2LoggerDo(thread_control * control);

int stoertest1(int id, int spiStoerung);
void stoertest1Loop(int startID, int stopID,int spi);
int stoertest1Do(thread_control * ctl);
int stoertest1DoStoerung(thread_control * ctl);



}
