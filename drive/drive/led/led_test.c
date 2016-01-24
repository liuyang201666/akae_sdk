#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{	
	int fd = open("/dev/led", O_RDWR);
	char *a[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	int i;
	if (fd < 0) {
		printf("Open %s file error \n", "/dev/led");
		return -1;
	}
	//ssize_t write(int fd, const void *buf, size_t count)
	for (;;) {
		for (i = 0; i < 10; i++) {
			write(fd, a[i], 1);
			sleep(3);
		}
	}
	
	return 0;
}
