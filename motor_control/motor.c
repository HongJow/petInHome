#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>
#define PIN 1
#define Button 4

void motor_work(int value,int pin, int time);

int main(){
	if(wiringPiSetup() == -1)
		return -1;
	softPwmCreate(PIN, 0 ,200);	
	pinMode(Button,INPUT);
	while(1){
		if(digitalRead(Button) == 0 ){
			perror("motor worked\n");
			motor_work(5,PIN,600);
			motor_work(0,PIN,2400);
			motor_work(25,PIN,600);
			motor_work(0,PIN,00);	
		}
	}
	return 0;
}

void motor_work(int value, int pin, int time){
	softPwmWrite(pin,value);
	delay(time);
}


