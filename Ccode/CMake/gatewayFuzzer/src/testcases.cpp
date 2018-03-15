#include "filter.h"
#include "canSocket.h"
#include "util.h"
#include "testcases.h"

int test::Sending(const char * iface)
{
    std::cout<<"\n-----\ntestSending(const char * iface)\n";
    canSocket * sender = new canSocket(iface);
    sender->canSend(0x01,8,("abcdefgh"),false);
    delete sender;
    std::cout<<"done\n-----\n";
    return 0;
}

int test::SendRecievePair(const char * iface_sender, const char * iface_reciever)
{
    std::cout<<"\n-----\ntestSendRecievePair(const char * iface_sender, const char * iface_reciever)\n";
    std::cout<<"from " << iface_sender<< " to " << iface_reciever<<"\n";
    canSocket * sender = new canSocket(iface_sender);
    canSocket * reciever = new canSocket(iface_reciever);
    sender->canSend(0x7ff,8,("abcdefgh"),false);
    can_frame response;
    reciever->canRecieveOne(&response,0);
    util::printCANframe(response,iface_reciever);
    delete sender;
    delete reciever;
    std::cout<<"done\n-----\n";
    return 0;
}

int test::dynamicInputfilterTest(const char * iface_reciever)
{
    std::cout<<"\n-----\ndynamicInputfilterTest(const char * iface_reciever)\n";
    std::cout<<"filtered input"<< " to " << iface_reciever<<"\n";
    canSocket * reciever = new canSocket(iface_reciever);

    can_frame response;

    dynamicInputfilter * dynfilter = new dynamicInputfilter();

    while(true)
    {
        if(reciever->canRecieveOne(&response,MSG_DONTWAIT));
        else
        {
            //prevent busy waiting
            //top %CPU dropped from 95% to 0,7%
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            continue;
        }
        if(!dynfilter->testframe(&response))
        {
            util::printCANframe(response,iface_reciever);
        }
    }

    dynfilter->~dynamicInputfilter();
}

int test::Wait4SpezialMsgID(int id, const char * iface_reciever, int timeoutMS)
{
    std::cout<<"\n-----\ntestWait4SpezialMsg(const char * iface_reciever)\n";
    std::cout<<"from XXX"<< " to " << iface_reciever<<"\n";
    canSocket * reciever = new canSocket(iface_reciever);

    can_frame response;

    struct timeval start, temp;
    gettimeofday(&start, 0);

    while(true)
    {
        if(reciever->canRecieveOne(&response,MSG_DONTWAIT)) util::printCANframe(response,iface_reciever);
        else
        {
            //prevent busy waiting
            //top %CPU dropped from 95% to 0,7%
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            continue;
        }
        if(match::id(&response,id))
        {
            std::cout<<"\nMSG found id = 0x01C\n";
            break;
        }

        gettimeofday(&temp, 0);
        int time = ((temp.tv_usec - start.tv_usec)/1000)+((temp.tv_sec - start.tv_sec)*1000);

        if(time > (timeoutMS))
        {
            std::cout<<"\nTimout waiting for msg\n";
            delete reciever;
            std::cout<<"done\n-----\n";
            return 1;
            break;
        }
    }

    //EWOULDBLOCK?
    delete reciever;
    std::cout<<"done\n-----\n";
    return 0;
}


int test::SendAndWait4AllMsgID(const char *iface_sender, const char *iface_reciever)
{
    std::cout<<"\n-----\nSendAndWait4AllMsgID(const char *iface_sender, const char *iface_reciever)\n";
    std::cout<<"from " << iface_sender<< " to " << iface_reciever<<"\n";
    canSocket * reciever = new canSocket(iface_reciever);
    canSocket * sender = new canSocket(iface_sender);

    can_frame response;

    struct timeval start, temp;
    gettimeofday(&start, 0);

    for(int i = 1; i <= 0x07ff; i++)
    {
        std::cout<<"\n"<<i;

        int timeoutMS = 100;
        bool ignore = false;

        while(!ignore)
        {
            if(reciever->canRecieveOne(&response,MSG_DONTWAIT));//util::printCANframe(response,iface_reciever);
            else
            {
                //prevent busy waiting
                //top %CPU dropped from 95% to 0,7%
                std::this_thread::sleep_for (std::chrono::milliseconds(1));
                continue;
            }
            if(match::id(&response,i))
            {
                ignore = true;
                break;
            }

            gettimeofday(&temp, 0);
            int time = ((temp.tv_usec - start.tv_usec)/1000)+((temp.tv_sec - start.tv_sec)*1000);

            if(time > (timeoutMS))
            {
                break;
            }
        }
        //nachricht offenbar nicht teil des grundrauschens


        timeoutMS = 2000;

        bool found = false;

        sender->canSend(0x01,8,("abcdefgh"),false);


        while(!found)
        {
            if(reciever->canRecieveOne(&response,MSG_DONTWAIT));
            else
            {
                std::this_thread::sleep_for (std::chrono::milliseconds(1));
                continue;
            }
            if(match::id(&response,i))
            {
                found = true;
                break;
            }

            gettimeofday(&temp, 0);
            int time = ((temp.tv_usec - start.tv_usec)/1000)+((temp.tv_sec - start.tv_sec)*1000);

            if(time > (timeoutMS))
            {
                break;
            }
        }

        if(found) util::printCANframe(response,iface_reciever);


    }

    delete sender;
    delete reciever;


}
