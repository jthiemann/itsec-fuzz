#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int
main(void)
{
	int s;
	int nbytes;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	const char *ifname = "can1";

	if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}

	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
	
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}

	
	u_int32_t number = 0,error = 0;
	
	while(1)
    {
        int rbytes = recv(s,&frame, sizeof(struct can_frame),0);
        if(rbytes != 0)
        {
            
            u_int32_t content = 0;
  
            content += frame.data[0] << 8;
            content += frame.data[1];
            
            if(frame.can_id !=content) error++;
            
            //for(int i = 0; i< frame.can_dlc ; i++)
            {
              //  printf("\n %x",frame.data[0]);
            }
            
            number ++;
            //printf("\nRecieved: %d bytes content == %#x",rbytes,content);
            if(content == 0x07ff) break;
        }
        
    }
    
    printf("\nRecieved: %d msg",number);
    printf("\nDropped:  %d msg",0x07ff - number +1);
    printf("\nErrors:   %d msg\n",error);
    

	
	return 0;
}
