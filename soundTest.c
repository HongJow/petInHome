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
	char stringToSend[BUFFER_LENGTH];
	
	fprintf(stderr, "file open start\n");

	fd = open("/dev/ledtest_dev", O_RDWR);
	if(fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
	fprintf(stderr, "file open success\n");

	printf("type string:");
	scanf("%[^\n]%*c",stringToSend);
	printf("Writing messags [%s].\n", stringToSend);

	if(write(fd, stringToSend, strlen(stringToSend)) < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}

	printf("Reading from the device..\n");

	if(read(fd, receive, BUFFER_LENGTH) < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}


	ioctl(fd, COUNT_READ, receive);

	printf("The received message is: [%c]\n", receive);
	printf("End of program\n");

	return 0;


}
