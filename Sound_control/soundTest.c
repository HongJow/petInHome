#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define BUFFER_LENGTH 256
#define COUNT_READ	_IOR(100, 0, int)

static char receive[BUFFER_LENGTH];

int main(){
	int fd;

	fprintf(stderr, "file open start\n");

	fd = open("/dev/ledtest_dev", O_RDWR);
	if(fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
	fprintf(stderr, "file open success\n");


	printf("Reading from the device..\n");

	if(read(fd, receive, BUFFER_LENGTH) < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}

	printf("The received message is: [%s]\n", receive);
	printf("End of program\n");

	return 0;


}
