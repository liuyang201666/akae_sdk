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

DECLARE_WAIT_QUEUE_HEAD(sched_wq);
int sched_flag = 0;
static char key = NOKEY;

static char key_scan(void)
{
	//GPB8输出低电平，GPB9输出高电平
	writel(0, S3C2410_GPBDAT);
	writel((1 << 9) | readl(S3C2410_GPBDAT), S3C2410_GPBDAT);
	
	if(((readl(S3C2410_GPFDAT)) & (1 << 0)) == 0)
		return '1';
	else if(((readl(S3C2410_GPFDAT)) & (1 << 2)) == 0)
		return '2';

	//GPB8输出高电平，GPB9输出低电平
	writel(0, S3C2410_GPBDAT);
	writel((1 << 8) | readl(S3C2410_GPBDAT), S3C2410_GPBDAT);

	if(((readl(S3C2410_GPFDAT)) & (1 << 0)) == 0)
		return '3';
	else if(((readl(S3C2410_GPFDAT)) & (1 << 2)) == 0)
		return '4';
	
	return '5';
}

static irqreturn_t key_irq_isr(int irq, void *dev_id)
{
	sched_flag = 1;
	wake_up_interruptible(&sched_wq);	

	return IRQ_HANDLED;
}

static ssize_t keyirq_read(struct file *filp, char __user *buf, size_t count, loff_t *f_ops)
{
again:
	wait_event_interruptible(sched_wq, sched_flag != 0);
	
	//input mode
	writel((S3C2410_GPF0_INP | S3C2410_GPF2_INP), S3C2410_GPFCON);	

	//等待100毫秒
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(20);
	//mdelay(100);

	key = key_scan();

	//set GPB8、GPB9 are low, GPF0、GPF2 are interrupt mode.
	writel((~((1<<8) | (1<<9))) & readl(S3C2410_GPBDAT), S3C2410_GPBDAT);
	writel(((S3C2410_GPF0_EINT0) | (S3C2410_GPF2_EINT2)), S3C2410_GPFCON);

	if(key == '5' && sched_flag == 1)
	{
		sched_flag = 0;
		goto again;
	}

	put_user(key, buf);
	//copy_to_user(buf, &key, 1);
	key = NOKEY;
	sched_flag = 0;

	return 1;
}

int keyirq_open(struct inode *inode, struct file *filp)
{
	
	enable_irq(IRQ_EINT0);
	enable_irq(IRQ_EINT2);

	return 0;
}

int keyirq_release(struct inode *inode, struct file *filp)
{
	disable_irq(IRQ_EINT0);
	disable_irq(IRQ_EINT2);

	return 0;
}

int keyirq_ioctl(struct inode *inode, struct file *file,
		unsigned int cmd, unsigned long arg)
{
	return 0;
}

struct file_operations keyirq_fops = 
{
	.owner = THIS_MODULE,
	.read = keyirq_read,
	.open = keyirq_open, 
	.release = keyirq_release,
	.ioctl = keyirq_ioctl,
};

int __init keyirq_init(void)
{
	int rc;

	printk("_SDK2410 gpio key12 driver irq=%d\n", IRQ_EINT0);
	printk("_SDK2410 gpio key34 driver irq=%d\n", IRQ_EINT2);

	//设置GPF0与GPF2为中断模式
	writel((S3C2410_GPF0_EINT0 | S3C2410_GPF2_EINT2), S3C2410_GPFCON);
	//设置GPF管脚取消内部上拉电平
	writel(0xFF, S3C2410_GPFUP);
	//设置GPB8和GPB9管脚为输出模式
	writel((S3C2410_GPB8_OUTP | S3C2410_GPB9_OUTP), S3C2410_GPBCON);

	writel((~((1 << 8) | (1 << 9))) & readl(S3C2410_GPBDAT), S3C2410_GPBDAT);

	set_irq_type(IRQ_EINT0, IRQF_TRIGGER_FALLING); 
	set_irq_type(IRQ_EINT2, IRQF_TRIGGER_FALLING); 

	rc = request_irq(IRQ_EINT0, key_irq_isr, IRQF_DISABLED, "key1irq", NULL); /* 2.6.27 普通中断 flag为0 */
	if (rc)
	{
		printk("<1>keyirq 1 irq not registered. Err:%d\n", rc);
	}

	rc = request_irq(IRQ_EINT2, key_irq_isr, IRQF_DISABLED, "key2irq", NULL); /* ++ */
	if (rc)
	{
		printk("<1>keyirq 2 irq not registered. Err:%d\n", rc);
	}

	disable_irq(IRQ_EINT0);
	disable_irq(IRQ_EINT2);
	
	if ((rc = register_chrdev(keyirq_MAJOR, "key", &keyirq_fops)) < 0)
	{
		printk("keyirq: can't get major %d\n", keyirq_MAJOR);
		return -1;
	}

	return 0;
}

void __exit keyirq_exit(void)
{
	disable_irq(IRQ_EINT0);
	disable_irq(IRQ_EINT2);

	free_irq(IRQ_EINT0,NULL);
	free_irq(IRQ_EINT2,NULL);
	
	unregister_chrdev(keyirq_MAJOR, "key");

	printk("gpio module removed\n");
	return;
}

module_init(keyirq_init);
module_exit(keyirq_exit);
