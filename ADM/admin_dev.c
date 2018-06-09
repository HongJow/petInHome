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
#include<linux/string.h>

MODULE_LICENSE("GPL");

struct cdev my_cdev;
dev_t devno;
int major;
static char *msg = NULL;
static char *motor = NULL;
static char *sound = NULL;
static char *sonic = NULL;
int device_open(struct inode *pinode,struct file *pfile);

ssize_t device_motor_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
ssize_t device_sound_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
ssize_t device_sonic_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
ssize_t lcd_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
int ledtest_close(struct inode *pinode, struct file *pfile);

static struct file_operations device_motor_fops ={
		.owner = THIS_MODULE,
		.write = device_motor_write,
};
static struct file_operations device_lcd_fops ={
		.owner = THIS_MODULE,
		.read  = lcd_read,
};
static struct file_operations device_sound_fops = {
		.owner = THIS_MODULE,
		.write  = device_sound_write,
};
static struct file_operations device_sonic_fops = {
		.owner = THIS_MODULE,
		.write  = device_sonic_write,
};
struct file_operations device_fop ={
	.owner		= THIS_MODULE,
	.open			= device_open,
};

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int device_open(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "OPEN ledtest_dev\n");
	switch(MINOR(pinode->i_rdev)){
		case 0:pfile->f_op = &device_lcd_fops;break;
		case 1:pfile->f_op = &device_motor_fops;break;
		case 2:pfile->f_op = &device_sound_fops;break;
		case 3:pfile->f_op = &device_sonic_fops;break;
	}	
	if(pfile->f_op && pfile->f_op->open) return pfile->f_op->open(pinode,pfile);
	return 0;
}

ssize_t device_motor_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	printk("write motor\n");
	int cfu = copy_from_user(motor,buffer,length);
	if(cfu <0 ) printk("cfu err\n");
	printk("copy from : motor : %s, buffer : %s\n",motor,buffer);
	return 0;
}
ssize_t device_sound_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	printk("write sound\n");
	int cfu = copy_from_user(sound,buffer,length);

	printk("copy from : msg : %s, buffer : %s",sound,buffer);
	return 0;
}
ssize_t device_sonic_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	printk("write sonic\n");
	int cfu = copy_from_user(sonic,buffer,length);
	printk("copy from : msg : %s, buffer : %s",sonic,buffer);
	return 0;
}
ssize_t lcd_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	printk("send to lcd");

	strcpy(msg,motor); strcat(msg,",");
	strcat(msg,sound); strcat(msg,",");
	strcat(msg,sonic);
	printk("lcd_read : msg : %s",msg);
	int ctu = copy_to_user(buffer,msg,length);
	printk("copy to : msg : %s, buffer : %s",msg,buffer);
	return 0;
}

int ledtest_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT, "RELEASE ledtest_dev\n");
	return 0;
}

int __init ledtest_init(void){
	major = 255;
	register_chrdev(255,"admin",&device_fop);
	cdev_init(&my_cdev,&device_fop);
	my_cdev.owner = THIS_MODULE;
	
	msg = (char*) kmalloc(32,GFP_KERNEL);
	motor = (char*) kmalloc(32,GFP_KERNEL);
	sound = (char*) kmalloc(32,GFP_KERNEL);
	sonic = (char*) kmalloc(32,GFP_KERNEL);
	
	memset(sound,'\0',sizeof(sound));	
	memset(msg,'\0',sizeof(msg));
	memset(sonic,'\0',sizeof(sonic));
	memset(motor,'\0',sizeof(motor));

	if(msg != NULL) printk("msg = 0x%p\n",msg);
	
	if(cdev_add(&my_cdev, devno, 3)<0){
		printk("Device add err\n");
		return -1;	
	}

	printk("sudo insmod admin_dev.ko\n");
	printk("sudo mknod -m 666 /dev/(lcd, motor, sound, sonic) c 255 (0,1,2,3)\n");
	
	return 0;
}
void __exit ledtest_exit(void){
	printk(KERN_ALERT "Exit admin\n");
	if(msg) kfree(msg);
	unregister_chrdev(255,"admin");
	cdev_del(&my_cdev);
}

module_init(ledtest_init);
module_exit(ledtest_exit);

