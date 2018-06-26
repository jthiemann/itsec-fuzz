#include "util.h"
#include <stdlib.h>

int util::updateDMESG()
{
int status = system("../getSPICANX");
return status;
}

int util::setupPIN()
{
    return system("../setupPin0Output");
}

int util::setPIN(bool state)
{
    std::string cmd;
    cmd = "../setPin0 ";
    if(state) cmd+="1";
    else cmd +="0";
    return (system(cmd.c_str()));
}
std::string util::stdCANframe(can_frame frame)
{


    char buffer2[4];
    char buffer[4];
    std::string out = "";

    sprintf(buffer2," %X",frame.can_id);
    std::string id(buffer2,strlen(buffer2));
    out += id +"#";

    for(int i= 0; i < frame.can_dlc;i++)
    {
        sprintf(buffer," %X",frame.data[i]);
        std::string std(buffer,strlen(buffer));
        out += std +" ";
    }



    return out;
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

std::string util::charToHexString(uint8_t hex)
{
    char buffer[20];
    sprintf(buffer,"%X",hex);
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

util::easytimer::easytimer()
{
    reset();
}

void util::easytimer::reset()
{
    gettimeofday(&lasttime, 0);
}

int util::easytimer::timePassedms()
{
    gettimeofday(&temp, 0);
    return ((temp.tv_usec - lasttime.tv_usec)/1000)+((temp.tv_sec - lasttime.tv_sec)*1000);
}

bool util::easytimer::XmsPassed(int ms)
{
    if(ms <= timePassedms()) return true;
    return false;
}



