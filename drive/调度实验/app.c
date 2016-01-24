#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NOKEY 0
int main()
{
	int keys4_fd;
	char ret[2];

	keys4_fd = open("/dev/akae", O_RDONLY);
	if(keys4_fd <= 0)
	{
		printf("open akae device error!\n");
		return 0;
	}

	read(keys4_fd, ret, 1);

	printf("key=%c\n", ret[0]);

	close(keys4_fd);
	return 0;
}
