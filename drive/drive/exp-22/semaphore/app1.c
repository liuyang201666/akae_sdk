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
unsigned char write_buf[1] = {0xaa};
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
			write(fd,write_buf,sizeof(write_buf));
			sleep(3);
			read(fd,read_buf,sizeof(read_buf));
			if(read_buf[0]!=write_buf[0])
				printf("App1 error!yyyyyyyyyyyyyyyy\n");	
		}
	}
  	else
  	{
    		printf("Device open failure\n");
  	}
	return 0;
}
