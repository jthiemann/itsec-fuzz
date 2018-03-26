
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include "util.h"

#define arraylenght 0x7ff

namespace match {
bool id(can_frame * frame, int id);
int check(void);
}


class idmetrik
{
public:
    idmetrik(int id);
    ~idmetrik();



    void setblock();
    void setblockby(int blocktype);
    bool test(can_frame *frame);
    bool getblock() {return _blocked;}
    bool getid() {return _id;}
private:
    bool _blocked;
    int _id;
    int _counter;
    int _blockby;

    uint8_t msg[8];
    int dlc = -1;
};

class dynamicInputfilter
{
public:



    dynamicInputfilter();
    ~dynamicInputfilter();

    bool testframe(can_frame *frame);
    void setblockby(int id,int blocktype);

private:
    idmetrik *_array[arraylenght];
};


