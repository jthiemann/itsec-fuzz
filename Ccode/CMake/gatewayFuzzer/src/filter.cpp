
#include "filter.h"


bool match::id(can_frame *frame, int id)
{
    return(frame->can_id == id);
}

int match::check(void)
{
        return 0;
}


dynamicInputfilter::dynamicInputfilter()
{
    idmetrik * dummy = new idmetrik(0);

    for(int i = 0; i <0x800; i++)
    {
        _array[i] = dummy;
    }
}

dynamicInputfilter::~dynamicInputfilter()
{
    idmetrik * elem;
    for(int i = 1; i <0x800; i++)
    {
        elem = _array[i];
        if(elem->getid() != 0) elem->~idmetrik();
    }
    _array[0]->~idmetrik();
}

bool dynamicInputfilter::testframe(can_frame *frame)
{
    int id = frame->can_id;

    if(_array[id]->getid() == 0)
    {
        _array[id] = new idmetrik(id);
    }

    return _array[frame->can_id]->test(frame);
}

idmetrik::idmetrik(int id)
{
    _id = id;
    _blocked = false;
    _counter = 0;
}

idmetrik::~idmetrik()
{

}

void idmetrik::setblock()
{
    _blocked = true;
}

bool idmetrik::test(can_frame *frame)
{
    _counter++;
    if(_counter > 1) _blocked = true;
    return _blocked;
}
