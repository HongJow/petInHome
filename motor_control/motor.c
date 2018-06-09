#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PIN 25
#define Button 24

void motor_work(int value,int pin, int time);

int main(){
	int ret,fd;
	int work_count = 0;
	char strToSend[1024];
	fd = open("/dev/motor",O_RDWR);

	if(fd <0) perror("open err");

	if(wiringPiSetup() == -1)
		{perror("setup failed");			
		return -1;
	}
	softPwmCreate(PIN, 0 ,200);	
	pinMode(Button,INPUT);

	while(1){
	int btn = digitalRead(Button);
		if(btn == 0 ){
			work_count++;
			sprintf(strToSend,"%ld",work_count);
			perror("motor worked\n");
			motor_work(5,PIN,60);
			motor_work(0,PIN,100);
			motor_work(25,PIN,60);
			motor_work(0,PIN,100);
			write(fd,strToSend,strlen(strToSend));
			memset(&strToSend,0,sizeof(strToSend));
		}
	}
	return 0;
}

void motor_work(int value, int pin, int time){
	softPwmWrite(pin,value);
	delay(time);
}


