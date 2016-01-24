#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/irq.h> 
#include <asm/irq.h>
#include <mach/irqs.h> 
#include <mach/regs-irq.h> 
#include <mach/regs-gpio.h> 
#include <linux/io.h>
#include <linux/delay.h>


MODULE_AUTHOR("Bryan");
MODULE_DESCRIPTION("GPIO KEY TEST");
MODULE_LICENSE("GPL");

#define keyirq_MAJOR 230
#define NOKEY '0'

#define SDDATA0
#define SDDATA1
#define SDDATA2
#define SDDATA3
#define SDCLK
#define SDCMD


DECLARE_WAIT_QUEUE_HEAD(sched_wq);
int sched_flag = 0;
static char key = NOKEY;

static ssize_t sdCard_read(struct file *filp, char __user *buf, size_t count, loff_t *f_ops)
{

	return 1;
}

int sdCard_open(struct inode *inode, struct file *filp)
{

	return 0;
}

int sdCard_release(struct inode *inode, struct file *filp)
{

	return 0;
}

int sdCard_ioctl(struct inode *inode, struct file *file,
		unsigned int cmd, unsigned long arg)
{
	return 0;
}

struct file_operations keyirq_fops = 
{
	.owner = THIS_MODULE,
	.read = sdCard_read,
	.open = sdCard_open, 
	.release = sdCard_release,
	.ioctl = sdCard_ioctl,
};

int __init sdCard_init(void)
{


	return 0;
}

void __exit sdCard_exit(void)
{

	return;
}

module_init(sdCard_init);
module_exit(sdCard_exit);

