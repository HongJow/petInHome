#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include <stdlib.h>
#define BUFFER_LENGTH 256
#define COUNT_READ	_IOR(100, 0, int)

static char receive[BUFFER_LENGTH];

int main(){
	int fd;
	char* buffer;
	buffer = (char*)malloc(sizeof(char));
	fprintf(stderr, "file open start\n");

	fd = open("/dev/lcd", O_RDWR);
	read(fd,buffer,3);
	printf("End of program\n");

	return 0;


}
