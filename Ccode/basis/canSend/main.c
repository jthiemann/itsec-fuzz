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

#include <sys/time.h>


int
main(void)
{
	int s;
	int nbytes;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	const char *ifname = "vcan0";

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

	//t = clock() - t;
    //printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
	
	
    
    struct timeval start, end;
    gettimeofday(&start, 0);

    u_int32_t i;

    //short frame 0x07ff
    //extended frame 0x1fffffff
    
	for( i = 0; i <= 0x7ff; i++)
    {
	
        frame.can_id  = i;
        frame.can_dlc = 2;
        frame.data[0] = i >>8;
        frame.data[1] = i;

        nbytes = write(s, &frame, sizeof(struct can_frame));
        usleep(800); //ms
	// zwischen 700 und 800  us liegt die Grenz, ab der Pakete
	// verloren gehen bei 100000 Hz Busspeed
	// 800 == 2048 Pakete mit dlc 2 in < 1,9 sec
        //for(int k =0; k<50;k++);
    }
	printf("Wrote %d x %d bytes\n", nbytes,i);
    
    gettimeofday(&end, 0);
    printf("start %ld %ld \n",start.tv_sec,start.tv_usec);
    printf("start %ld %ld \n",end.tv_sec,end.tv_usec);
    
    int diff = end.tv_usec -start.tv_usec;
    
    printf("Time used == %f seconds\n",(((float)diff/1000000)+(end.tv_sec - start.tv_sec))); 
	
	return 0;
}
