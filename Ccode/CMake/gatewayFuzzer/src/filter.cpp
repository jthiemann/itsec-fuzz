
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

    if((id<=0) || (id >= arraylenght))
    {
        std::cout <<"out if range of 0x001-0x7ff" << std::to_string(id) << "\n";
        return false;
    }

    if(_array[id]->getid() == 0)
    {
        _array[id] = new idmetrik(id);
    }

    return _array[frame->can_id]->test(frame);
}

void dynamicInputfilter::setblockby(int id, int blocktype)
{
    if(_array[id]->getid() == 0)
    {
        _array[id] = new idmetrik(id);
    }

    if(!((id<=0) || (id >= arraylenght)))
    {
        _array[id]->setblockby(blocktype);
    }
}

std::string dynamicInputfilter::getReportRList()
{
    std::string retval = "Report for dynamic filter\n";
    retval.append("-----------------------------------------\n");
    retval.append("ID\tTOTAL\tUPDATE\n");

    for(int i = 0; i < arraylenght; i++)
    {
        idmetrik  elem = *_array[i];
        if( elem.getid() != 0)
        {

            retval.append(util::toHexString(i)+"\t");
            retval.append(std::to_string(elem.getRCount())+"\t");
            retval.append(std::to_string(elem.getRUpdates())+"\n");
        }
    }
    retval.append("-----------------------------------------\n");
    return retval;
}

idmetrik::idmetrik(int id)
{
    _id = id;
    _blocked = false;
    _counter = 0;
    _updates = 0;
    msg[8];
    dlc = -1;
    _blockby = update;



}

idmetrik::~idmetrik()
{

}

void idmetrik::setblock()
{
    _blocked = true;
}

void idmetrik::setblockby(int blocktype)
{
    _blockby = blocktype;
}


bool idmetrik::test(can_frame *frame)
{
    //return true if the msg is blocked

    _counter++; //overall Counter

    if(_blockby == once)
    {
        if(_counter > 1) _blocked = true;
        return _blocked;
    }

    if(_blockby == update)
    {


        if(dlc == -1)
        {
            //first time
            dlc =  frame->can_dlc;
            for(int i = 0; i < dlc; i++)
            {
                msg[i] = frame->data[i];
            }
            _updates++;
            return false;
        }
        else
        {
            // change?
            if(dlc != frame->can_dlc)
            {
                dlc =  frame->can_dlc;
                for(int i = 0; i < dlc; i++)
                {
                    msg[i] = frame->data[i];
                }
                _updates++;
                return false;
            }
            else
            {
                bool change = true;
                for(int i = 0; i < dlc; i++)
                {
                    if(msg[i] != frame->data[i]) change = false;
                    msg[i] = frame->data[i];
                }
                if(!change) _updates++;
                return change;
            }

        }
    }

    if(_blockby == allways) return true;


}
