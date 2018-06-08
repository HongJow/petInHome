#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/fs.h>

#include <lcd.h>

#define MY_MAJOR 200
#define MY_MINOR 0
#define MY_DEV_COUNT 2

struct cdev my_cdev;
//struct cdev *my_cdev;
static char *msg = NULL;
dev_t devno;
int major;

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int simple_open (struct inode *pinode, struct file *pfile)
{
	printk("Open simple char drv\n");
	return 0;
}

ssize_t simple_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk("Read simple char drv\n");

	if(strcmp(msg, "test") == 0) {
	    copy_to_user(buffer, "GET TEST", length);
	}
	else {
	    copy_to_user(buffer, "NOT TEST", length);
	}
	return 0;
}

ssize_t simple_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk("Write simple char drv\n");

	int err;

	if(err = copy_from_user(msg, buffer, length)) {
	    //return err;
	}	

	return length;
}

int simple_close (struct inode *pinode, struct file *pfile)
{
	printk("Release simple char drv\n");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = simple_open,
	.read = simple_read,
	.write = simple_write,
	.release = simple_close,
};

int __init simple_init(void)
{
	printk("Init simple char drv\n");
	register_chrdev(240, // Major number
			"Simple char drv", // Driver name
			&fop);
	return 0;
}

int __init simple_init2(void)
{
	//dev_t devno;
	unsigned int count = MY_DEV_COUNT;
	int err;
	//int major = MY_MAJOR;
	//int major = 0;

	if(major) {
		devno = MKDEV(major, MY_MINOR);
		register_chrdev_region(devno, count, "Simple char drv");
	}
	else {
		alloc_chrdev_region(&devno, MY_MINOR, count, "Simple char drv");
		major = MAJOR(devno);
	}

	cdev_init(&my_cdev, &fop);
	my_cdev.owner = THIS_MODULE;
	err = cdev_add(&my_cdev, devno, count);
	
	if(err < 0) {
		printk("Device add error\n");
		return -1;
	}

	printk("Hello World. This is my first char dev.\n");
	printk("'mknod /dev/simple_chr_drv c %d 0'.\n", MAJOR(devno));

	msg = (char *)kmalloc(32, GFP_KERNEL);
	if (msg != NULL)
		printk("malloc allocator address: 0x%p\n", msg);

	return 0;
}

void __exit simple_exit(void)
{
	printk("Exit simple char drv\n");
	unregister_chrdev(240, "Simple char drv");
}

void __exit simple_exit2(void)
{
	//dev_t devno;
	printk("Goodbye\n");
	
	if(msg) {
		kfree(msg);
	}
	//devno = MKDEV(MY_MAJOR, MY_MINOR);
	unregister_chrdev_region(devno, MY_DEV_COUNT);
	cdev_del(&my_cdev);
	//cdev_del(my_cdev);
}	

module_init(simple_init2);
module_exit(simple_exit2);
