#include "util.h"
#include <stdlib.h>

int util::updateDMESG()
{
int status = system("../getSPICANX");
return status;
}

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

std::string util::toHexString(int hex)
{
    char buffer[20];
    sprintf(buffer,"0x%X",hex);
    std::string out(buffer,strlen(buffer));
    return out;
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
