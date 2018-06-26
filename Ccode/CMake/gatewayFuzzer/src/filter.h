
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include "util.h"

#include <vector>

#define arraylenght 0x800

#pragma once

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

    int getRCount() {return _counter;}
    int getRUpdates() {return _updates;}

private:
    bool _blocked;
    int _id;
    int _counter;
    int _updates;
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
    void setStaticBlockList(int spi);
    std::string getReportStatistic();
    std::string getReportRList();

private:
    idmetrik ** _array;//[arraylenght];
    //std::vector<idmetrik *> _array[arraylenght];


    //static blocklists

    int can0CycleIds[6] =
    {0x531,
    0x571,
    0x621,
    0x65D,
    0x65F,
    0x00};

    int can1CycleIds[6] =
    {0x531,
    0x571,
    0x621,
    0x65D,
    0x65F,
    0x00};

    int can2CycleIds[6] =
    {0x320,
    0x51A,
    0x520,
    0x570,
    0x5D2,
    0x7D0};






};




