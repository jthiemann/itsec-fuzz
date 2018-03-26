#include "util.h"



int util::printCANframe(can_frame frame,const char * iface)
{
        printf("\n%s  0x%03X [%X]",iface,frame.can_id, frame.can_dlc);
        for(int i= 0; i < frame.can_dlc;i++)
        {
            printf(" %X",frame.data[i]);
        }
        printf("\n");
        return 0;
}



void util::deepcopyCANframe(can_frame src, can_frame dest)
{
    dest.can_dlc = src.can_dlc;
    dest.can_id = src.can_id;

    for(int i = 0; i < dest.can_dlc; i++)
    {
        dest.data[i] = src.data[i];
    }
}
