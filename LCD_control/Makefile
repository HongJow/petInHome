obj-m += LCD.o

KERNEL_DIR = /lib/modules/4.9.80-v7+/build

CC = arm-linux-gnueabihf-gcc

all : 
	make -C $(KERNEL_DIR) M=$(PWD) modules
