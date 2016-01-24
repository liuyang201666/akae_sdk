#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NOKEY '0'
char *arry[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
int main()
{
	int fd;
	int uiNum = 0;
	fd = open("/dev/led", O_RDWR);
	if(fd <= 0)
	{
		printf("open Led device error!\n");
		return 0;
	}
	printf("open Led device Success !\n");

	while(1)
	{
		printf("Input uiNum: \n");
		fflush(stdout);
		scanf("%d", &uiNum);
		if(uiNum < 10)
 		{
			printf("Input is :%d \n", uiNum);
			write(fd, arry[uiNum], 1);
			sleep(1);
		}
		else
		{
			printf("INVALUE NUM !\n");
			break;
		}
		usleep(100000);
	}
	close(fd);
	return 0;
}
