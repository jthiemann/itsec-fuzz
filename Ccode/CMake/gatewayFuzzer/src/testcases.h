
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>

namespace test {

int Sending(const char * iface);
int SendRecievePair(const char * iface_sender, const char * iface_reciever);
int Wait4SpezialMsg(const char * iface_reciever, int timeoutMS);

}
