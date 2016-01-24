#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>


#define LEDMAJOR 241
#define LED_SEG		0x20800080

unsigned char seg_num[] = {
//	0x3f,0x06,0x5b,0x4f,    /* //0~3*/
//	0x66,0x6d,0x7d,0x07,    /*//4~7*/
//	0x7f,0x6f,0x77,0x7c,   /* //8~b*/
//	0x39,0x5e,0x79,0x71,
//	0x00/* other error num */
//	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
	0x3f,0x30,0x6d,0x4f,    /* //0~3*/
	0x4c,0x5b,0x7b,0x0e,    /*//4~7*/
	0x7f,0xaf,0x77,0x7c,   /* //8~b*/
	0x39,0x5e,0x79,0x71,
	0x00/* other error num */
};

int led_open(struct inode *inode, struct file *flip)
{
	int major, minor;
	minor = MINOR(inode->i_rdev);
	major = MINOR(inode->i_rdev);
	printk("led_open ok major = %d, minor = %d\n", major, minor);
	return 0;
}

int led_write(struct file *flip, const char __user *buf, size_t count, loff_t *f_pos)
{
	void *seg;
	//struct inode *inode = flip->f_path.dentry->d_inode;
	//int major, minor;
	//minor = MINOR(inode->i_rdev);
	//major = MAJOR(inode->i_rdev);
	//printk("led_write ok major  = %d, minor = %d\n", major, minor);
	seg = ioremap(LED_SEG, 1);
	iowrite8(0, seg);
	iowrite8(seg_num[*buf - 0x30], seg);
	return 0;
}

struct file_operations niu_fops = {
	.owner = THIS_MODULE,
	.open =led_open,
	.write = led_write,
};

int __init led_init(void)
{
	int rc;
	printk("Test led\n");
	rc = register_chrdev(LEDMAJOR, "led", &niu_fops);
	if (rc < 0) {
		printk("register char dev error \n");
		return -1;
	}
	printk(" register char dev ok!\n");
	return 0;
}
void __exit led_exit(void)
{
	unregister_chrdev(LEDMAJOR, "led");
	printk("module exit\n");
	return;
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("niuxin");
MODULE_DESCRIPTION("led driver");
MODULE_LICENSE("GPL");

