
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include <thread>
#include <mutex>
#include "canSocket.h"
#include "filter.h"





namespace th_test_sven{

void threadedTest();
void testLOG(canSocket * Socket, int spi);
//jene Namen
void log2Logger();
void log2LoggerDo(thread_control * control);

int stoertest1(int id, int spiStoerung, canSocket *spi0, canSocket *spi1, canSocket *spi2, canSocket *spiSend);
void stoertest1Loop(int startID, int stopID, int spiStoerung);
int stoertest1Do(thread_control * ctl);
int stoertest1DoStoerung(thread_control * ctl);

}
