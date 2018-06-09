#include <wiringPi.h>
#include <stdio.h>
#include <lcd.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define LCD_RS	11
#define LCD_E		10
#define LCD_D4	29
#define LCD_D5	28
#define LCD_D6	27
#define LCD_D7	26

int main(){
	wiringPiSetup();
	int lcd;
	int fd;
	fd = open("/dev/lcd",O_RDWR);
	if(lcd = lcdInit(2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0)){
		printf("lcd init failed!!\n");
		return -1;
	}
	else{
			int count = 0;
			lcdPosition(lcd,0,0);
		while(1){
			char* token;
      char buf[10];
      int ret;
			char lcd_line[17];
			read(fd,buf,sizeof(buf));
			ret = strlen(buf);
			buf[10]= '\0';
			printf("buffer : %s ::: %d chars\n",buf,ret);
			token = strtok(buf,",");	
			switch(count){
				lcdClear(lcd);
				case 0 : 
					lcd_line[17];
					strcpy(lcd_line,"feed : ");
					strcat(lcd_line,token);
					lcdPuts(lcd,lcd_line);
					sleep(3);
					lcdClear(lcd);
					count= (count+1)%3;
					break;
				case 1:
					token = strtok(NULL,",");
					lcd_line[17];
					strcpy(lcd_line,"barked : ");
					strcat(lcd_line,token);
					lcdPuts(lcd,lcd_line);
					sleep(3);
					lcdClear(lcd);
					count= (count+1)%3;
					break;
				case 2:
					token = strtok(NULL,",");
					token = strtok(NULL,",");
					lcd_line[17];
					strcpy(lcd_line,"forbid : ");
					strcat(lcd_line,token);
					lcdPuts(lcd,lcd_line);
					sleep(3);
					lcdClear(lcd);
					count= (count+1)%3;
					break;
			}	
		}
	}
}
