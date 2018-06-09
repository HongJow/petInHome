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
#define SOUND_DOUT_PIN 3 
#define DEV_NAME "sound_dev"
#define DEV_NUM 250
#define COUNT_READ _IOR(100, 0, int)

MODULE_LICENSE("GPL");

static int count;
static int lock = 0;
struct task_struct *task;
struct cdev my_cdev;
dev_t devno;
int major;
static char *msg = NULL;

int sound_thread(void *data);

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int sound_open(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "OPEN sound_dev\n");
	return 0;
}

ssize_t sound_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	printk("Write simple char drv\n");
	
	if(copy_from_user(msg, buffer, length)){
		printk("Write error\n");
		return -1;
	}
	printk("copyfrom success\n");
	return length;
}

ssize_t sound_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	//printk("Read sound drv\n");

	kthread_stop(task);

	//printk("count: %d\n", count);

	char StrCount[10];
	sprintf(StrCount,"%ld",count);
	StrCount[10] = '\0';
	//printk("StrCount: %s\n", StrCount);

	copy_to_user(buffer, StrCount, length);
	
	//printk("copyto success : StrCount : %s, buffer: %s\n", StrCount, buffer);
	
	task = kthread_run(sound_thread, NULL, "sound thread");

	return 0;
}

int sound_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT, "RELEASE sound_dev\n");
	return 0;
}

int sound_thread(void *data){
	int seq = 0;
	while(1){
		int value = gpio_get_value(SOUND_DOUT_PIN);
		if(value == 0){
			count++;
		}
		if(count>50){
			count = 0;
		}
		usleep_range(1000,1000);

		if(kthread_should_stop()) break;

	}
	return 0;

}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = sound_open,
	.read = sound_read,
	.write = sound_write,
	.release = sound_close,
};

int __init sound_init(void){
	printk(KERN_ALERT "INIT sound_dev\n");
	
	devno = MKDEV(major, 0);
	register_chrdev_region(devno, 1, "sound_dev");
	cdev_init(&my_cdev, &fop);
	my_cdev.owner = THIS_MODULE;

	msg = (char *)kmalloc(32, GFP_KERNEL);

	if(cdev_add(&my_cdev, devno, 1) < 0){
		printk("Device add error\n");
		return -1;
	}
	
	if(gpio_request(SOUND_DOUT_PIN, "SOUND_DOUT_PIN") < 0){
		printk("gpio_request fail\n");
	}
	if(gpio_direction_input(SOUND_DOUT_PIN) < 0){
		printk("gpio_direction_input fail\n");
	}
	
	task = kthread_run(sound_thread, NULL, "sound thread");

	return 0;
}
void __exit sound_exit(void){
	printk(KERN_ALERT "Exit sound_dev\n");
	if(msg){
		kfree(msg);
	}
	kthread_stop(task);
	gpio_free(3);
	unregister_chrdev(250, DEV_NAME);
	cdev_del(&my_cdev);
}

module_init(sound_init);
module_exit(sound_exit);

