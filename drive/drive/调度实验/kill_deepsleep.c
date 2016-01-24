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

int __init
serial_init(void)
{
	struct task_struct *p;
	int rc;
	for_each_process(p)
	{
		rc = strncmp(p->comm, "app", 3);
		if(rc == 0)
		{
			wake_up_process(p);	

			printk("wakeup!\n");
		}

		if(!p->state)
			printk("%s, pid = %d\n", p->comm, p->pid);
	}

	return -1; //return 0; 返回-1的时候无法加载模块，但是init中的东西会被执行。
}

void __exit
serial_exit(void)
{
	printk("module exit\n");
	return;
}

module_init(serial_init);
module_exit(serial_exit);
