#include "canSocket.h"


canSocket::canSocket(const char * ifname)
{       
        _ifname = ifname;

        if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
                perror("Error while opening socket");
                error =  -1;
        }



        strcpy(ifr.ifr_name, _ifname);
        ioctl(s, SIOCGIFINDEX, &ifr);

        addr.can_family  = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        printf("%s at index %d\n", _ifname, ifr.ifr_ifindex);

        if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Error in socket bind");
                error = -2;
        }

}
bool canSocket::canSend(int id, int length, char *data, bool extended)
{
    if((id < 0) || (( id > 0x07ff) && (!extended) ))
    {
        printf("\nid out of range");
        return false;
    }
    send_frame.can_id = id;
//ToDo max lengt?
    if((length < 1) || length > 8)
    {
        printf("\n lenght not supported, be sure to also match the array length");
        return false;
    }

    send_frame.can_dlc = length;

    for(int i = 0; i < length; i++)
    {
        send_frame.data[i] = data[i];
    }
    nbytes = 0;
    nbytes = write(s, &send_frame, sizeof(struct can_frame));

    if(nbytes != 0) return true;

    return false;
}

bool canSocket::canSendBatch()
{
    //short frame 0x07ff
    //extended frame 0x1fffffff
    int i;
    for( i = 0; i <= 0x7ff; i++)
    {

        send_frame.can_id  = i;
        send_frame.can_dlc = 2;
        send_frame.data[0] = i >>8;
        send_frame.data[1] = i;

        nbytes = write(s, &send_frame, sizeof(struct can_frame));
        usleep(800); //ms
        // zwischen 700 und 800  us liegt die Grenz, ab der Pakete
        // verloren gehen bei 100000 Hz Busspeed
        // 800 == 2048 Pakete mit dlc 2 in < 1,9 sec
        //for(int k =0; k<50;k++);
    }
        printf("Wrote %d x %d bytes\n", nbytes,i);



    return 0;
}



bool canSocket::canRecieveWait4ExpectedMSG(can_frame input_frame, can_frame * outputframe)
{
    //input_frame is the frame send into the network
    int rbytes = recv(s,&recv_frame, sizeof(struct can_frame),0);
    if(rbytes != 0)
    {
        printf("recieved one msg");
        return true;
    }
    return false;
}

bool canSocket::canRecieveOne(can_frame * frame, int flags)
{
    int rbytes = recv(s,frame, sizeof(struct can_frame),flags);
    if(rbytes > 0)
    {
        return true;
    }
    if(rbytes == -1)
    {
       //printf("\nold msg\n");
    }
    return false;
}

