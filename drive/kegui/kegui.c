/********************************************
 *      Funtion :modele example
 *      2012-07-06
 *
 *
 * ******************************************/

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/timer.h>

static  int count =0;

MODULE_AUTHOR("Asmcos");
MODULE_DESCRIPTION("module example");
MODULE_LICENSE("GPL");

module_param(count, int, 0);

struct timer_list akae_timer;

void akae_timer_fn (unsigned long arg)
{
        printk("akae_timer_fn\n");

        akae_timer.expires += 200;
        add_timer (&akae_timer);
}

int __init
akae_init(void)
{
        printk("Hello, world !\n count=$%d\n", count);

        init_timer(&akae_timer);

        akae_timer.expires = jiffies +200;
        akae_timer.function = akae_timer_fn;

        add_timer(&akae_timer);

        printk("ok !\n");

        return 0;

}

void __exit
akae_exit(void)
{
        del_timer_sync(&akae_timer);
        printk("nidyke exut\n");
        return ;
}

module_init(akae_init);
module_exit(akae_exit);

