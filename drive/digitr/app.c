#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NOKEY '0'
int main()
{
	int keys4_fd;
	char ret[2];
	char tmp[2];

	//keys4_fd = open("/dev/key", O_WRONLY);
	keys4_fd = open("/dev/key", O_RDONLY);
	if(keys4_fd <= 0)
	{
		printf("open key device error!\n");
		return 0;
	}
	while(1)
	{
		read(keys4_fd, ret, 1);
		if(ret[0] != NOKEY)
		{
//			write(keys4_fd, tmp, 1);
			printf("key=%c\n", ret[0]);
		}
		usleep(100000);
	}
	closed(keys4_fd);
	return 0;
}
