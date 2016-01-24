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
unsigned char write_buf[1] = {0x55};
int main(void)
{
	int fd;
	char rd_ch[BUFFER_LEN];
  
	fd = open("/dev/globalfifo", O_RDWR );
	if (fd !=  - 1)
	{
		while(1)
		{
			write(fd,write_buf,sizeof(write_buf));
			printf("App write%x\n",write_buf[0]);
			sleep(1);
		}
	}
  	else
  	{
    		printf("Device open failure\n");
  	}
	return 0;
}
