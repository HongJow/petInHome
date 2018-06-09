#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<softPwm.h>
#include<wiringPi.h>
#include<stdlib.h>
#define PIN 23

#define BUFFER_LENGTH 256
#define COUNT_READ	_IOR(100, 0, int)

static char receive[BUFFER_LENGTH];
int motor_work(int a, int b , int c);
int main(){
	int fd;
	int fd_sound_motor;
	wiringPiSetup();
	softPwmCreate(PIN,0,200);
	fprintf(stderr, "file open start\n");

	fd = open("/dev/sound_dev", O_RDWR);
	if(fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
	fd_sound_motor = open("/dev/sound",O_RDWR);
	if(fd_sound_motor<0) {
		perror("sound motor fd failed: "); 
		return -1;
	}
//	printf("Reading from the device..\n");
	int work_count = 0;
while(1){
//	printf("start while\n");
	if(read(fd, receive, BUFFER_LENGTH) < 0){
		perror("Failed to read the message from the device.!!!!");
		return errno;
	}	//receive= char* type
	int val=atoi(receive);
//	perror("atoi :");
//	printf("%d\n",val);
	if(val>40){
		printf("bark stack : %d",val);
		work_count++;
		char buffer[10];
		sprintf(buffer,"%ld",work_count);
		motor_work(40,PIN,2400);
		motor_work(0,PIN,10);
		write(fd_sound_motor,buffer,sizeof(buffer));
		printf("buf : %s\n",buffer);
		sleep(2);
		}
	}
}
int motor_work(int value, int pin, int time){
	softPwmWrite(pin,value);
	delay(time);
}
