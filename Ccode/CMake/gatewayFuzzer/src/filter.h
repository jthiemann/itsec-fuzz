
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>
#include "util.h"

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
    bool test(can_frame *frame);
    bool getblock() {return _blocked;}
    bool getid() {return _id;}
private:
    bool _blocked;
    int _id;
    int _counter;
};

class dynamicInputfilter
{
public:
    dynamicInputfilter();
    ~dynamicInputfilter();

    bool testframe(can_frame *frame);

private:
    idmetrik *_array[0x7ff];
};


