/*********************************************
 * Function: module example
 * Author: asmcos@gmail.com
 * Date: 2005-08-24
 * $Id: kegui.c, v 1.6 2006/06/22 13:20:50 asmcos Exp $
 *********************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h> /* copy_to_user, copy_from_user */
#include <linux/fs.h>
#include <linux/sched.h>

#define AKAE_MAJOR 240

MODULE_AUTHOR("Asmcos");
MODULE_DESCRIPTION("module example ");
MODULE_LICENSE("GPL");



int my_open(struct inode *inode, struct file *filp)
{
	int major, minor;
	minor = MINOR(inode->i_rdev);
	major = MAJOR(inode->i_rdev);

	printk("my_open ok major=%d, minor=%d\n", major, minor);

	return 0;
}

ssize_t my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	//current->state = TASK_INTERRUPTIBLE;
	current->state = TASK_UNINTERRUPTIBLE;
	printk("task name = %s, pid=%d call me!\n", current->comm, current->pid);

	schedule();
	return count;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{

	return count;
}

int my_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{

	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	return 0;
}


//结构体初始化
struct file_operations my_fops ={
	.owner = THIS_MODULE,
	.open  = my_open,
	.read  = my_read,
	.write = my_write,
	.release = my_release,
	.ioctl = my_ioctl,
};

int __init
serial_init(void)
{
	int rc;
	
	rc = register_chrdev(AKAE_MAJOR, "akae", &my_fops);
	if(rc < 0)
	{
		printk("register %s char dev error\n", "akae");
		return -1;
	}

	printk("ok!\n");

	return 0;
}

void __exit
serial_exit(void)
{
	unregister_chrdev(AKAE_MAJOR, "akae");
	printk("module exit\n");
	return;
}

module_init(serial_init);
module_exit(serial_exit);
