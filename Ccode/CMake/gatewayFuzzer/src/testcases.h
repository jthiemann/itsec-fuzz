
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>

namespace test {

int config(const char * iface00, const char * iface01, const char * iface10);
int Sending(const char * iface);
int SendRecievePair(const char * iface_sender, const char * iface_reciever);
int Wait4SpezialMsgID(int id, const char * iface_reciever, int timeoutMS);
int SendAndWait4AllMsgID(const char * iface_sender, const char * iface_reciever);
int dynamicInputfilterTest(const char * iface_reciever);
int detectCanDown(const char * iface_reciever);
}
