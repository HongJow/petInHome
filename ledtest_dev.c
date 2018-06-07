#include<linux/gpio.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/delay.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/slab.h>

#define GPIO1 18
#define DEV_NAME "ledtest_dev"
#define DEV_NUM 240
#define COUNT_READ _IOR(100, 0, int)

MODULE_LICENSE("GPL");

static int count = 0;
struct task_struct *task;
struct cdev my_cdev;
dev_t devno;
int major;
static char *msg = NULL;

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int ledtest_open(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "OPEN ledtest_dev\n");
	return 0;
}

ssize_t ledtest_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	printk("Write simple char drv\n");

	if(copy_from_user(msg, buffer, length)){
		printk("Write error\n");
		return -1;
	}
	return length;
}

ssize_t ledtest_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	printk("Read ledtest drv\n");

	if(strcmp(msg, "test") == 0){
		copy_to_user(buffer, "GET TEST", length);
	}
	else{
		copy_to_user(buffer, "NOT TEST", length);
	}
	return 0;
}


long ledtest_ioctl(struct file *pfile, unsigned int command, unsigned long arg)
{
	printk(KERN_ALERT "Read ledtest drv\n");
	
	char* counnnt;
	counnnt[0] = '0' + count;
	counnnt[1] = '\0';	
	
	printk("counnnt : %s\n", counnnt);

	switch(command){
		case COUNT_READ:
			printk("copy_to_user start\n");
			copy_to_user((void*)arg, (void*)counnnt, (ssize_t)2);
			printk("copy_to_user done\n");
			break;
		default:
			printk("Not found command..\n");
			break;
	}
	
	return 0;
}
int ledtest_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT, "RELEASE ledtest_dev\n");
	return 0;
}

int sound_thread(void *data){
	
	while(1){
		int value = gpio_get_value(3);
		printk("gpio value = %d\n", value);
		
		int x = 0;
		if(value == 0){
			count++;
		}
		while(x < 500000){
			x++;
		}
		//delay(10);
		usleep_range(1000,1000);
		if(kthread_should_stop())

			break;

	}

}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = ledtest_open,
	.read = ledtest_read,
	.write = ledtest_write,
	.unlocked_ioctl = ledtest_ioctl,
	.release = ledtest_close,
};

int __init ledtest_init(void){
	printk(KERN_ALERT "INIT ledtest_dev\n");
	
	alloc_chrdev_region(&devno, 0, 1, "ledtest_dev");
	major = MAJOR(devno);
	cdev_init(&my_cdev, &fop);
	my_cdev.owner = THIS_MODULE;

	msg = (char *)kmalloc(32, GFP_KERNEL);

	if(cdev_add(&my_cdev, devno, 1) < 0){
		printk("Device add error\n");
		return -1;
	}
	
	if(gpio_request(3, "3") < 0){
		printk("gpio_request fail\n");
	}
	if(gpio_direction_input(3) < 0){
		printk("gpio_direction_input fail\n");
	}
	task = kthread_run(sound_thread, NULL, "sound thread");

	return 0;
}
void __exit ledtest_exit(void){
	printk(KERN_ALERT "Exit ledtest_dev\n");
	if(msg){
		kfree(msg);
	}
	kthread_stop(task);
	gpio_free(3);
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(ledtest_init);
module_exit(ledtest_exit);

