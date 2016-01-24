/*======================================================================
    A test program in userspace   
    This example is to introduce the ways to use "select"
     and driver poll                 
      
    The initial developer of the original code is Baohua Song
    <author@linuxdriver.cn>. All Rights Reserved.
======================================================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define FIFO_CLEAR 0x1
#define BUFFER_LEN 1
unsigned char read_buf[1];
int main(void)
{
	int fd;
	char rd_ch[BUFFER_LEN];
  
	fd = open("/dev/base", O_RDWR );
	if (fd !=  - 1)
	{
		while(1)
		{
			read(fd,read_buf,sizeof(read_buf));
			printf("App1 read%x\n",read_buf[0]);
			sleep(1);	
		}
	}
  	else
  	{
    		printf("Device open failure\n");
  	}
	return 0;
}
