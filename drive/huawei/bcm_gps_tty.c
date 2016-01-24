/******************************************************************************
 * Copyright (C) 2015 Broadcom Corporation
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 ******************************************************************************/
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/timerqueue.h>
#include <linux/rtc.h>
#include <linux/alarmtimer.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/posix-timers.h>
#include <linux/workqueue.h>
#include <linux/freezer.h>
#include <linux/ktime.h>

//TODO: this is temporary
#define PORT_NAME "/dev/ttyAMA3"
//1 ms = 1000000 ns
#define MS_TO_NS(x)	(x * 1E6L)

#define USE_HRTIMER 1
#define USE_HRTIMER_RESTART 0
#define BCM_TTY_DEBUG 1

struct bcm_tty_priv
{
	struct file *tty;
	struct miscdevice misc;
	int mcu_req;
	int mcu_resp;
	//int host_req;
};

enum txalarmtimer_type {
	TXALARM_REALTIME,
	TXALARM_MONOTONIC,
	TXALARM_NUMTYPE,
};

/*
 * bcm4773_hello - wakeup chip by toggling mcu_req while monitoring mcu_resp to check if awake
 *
 */
static bool bcm477x_hello(struct bcm_tty_priv *priv)
{
	int count=0, retries=0;

	gpio_set_value(priv->mcu_req, 1);
	printk(KERN_INFO "[GPSTTYCHAR]: gps_tty MCU_REQ = 1\n");

	// With hardware flow control this is not needed but this can be keep for debug purposes to make sure
	// mcu req resp pin is toggled properly
	while (!gpio_get_value(priv->mcu_resp)) 
	{
		if (count++ > 100) {
			gpio_set_value(priv->mcu_req, 0);
			printk("MCU_REQ_RESP timeout. MCU_RESP(gpio%d) not responding to MCU_REQ(gpio%d)\n", 
					priv->mcu_resp, priv->mcu_req);
			return false;
		}

		mdelay(1);

		/*if awake, done */
		if (gpio_get_value(priv->mcu_resp)) break;

		if (count%20==0 && retries++ < 3) {
			gpio_set_value(priv->mcu_req, 0);
			mdelay(1);
			gpio_set_value(priv->mcu_req, 1);
			mdelay(1);
		}
	}
	
	return true;
}

/*
 * bcm4773_bye - set mcu_req low to let chip go to sleep
 *
 */
static void bcm477x_bye(struct bcm_tty_priv *priv)
{
    
	gpio_set_value(priv->mcu_req, 0);
    printk(KERN_INFO "[GPSTTYCHAR]: gps_tty MCU_REQ = 0\n");
}

/**
 * struct txalarm - Alarm timer structure
 * @node:       timerqueue node for adding to the event list this value
 *              also includes the expiration time.
 * @period:     Period for recuring alarms
 * @function:   Function pointer to be executed when the timer fires.
 * @type:       Alarm type (BOOTTIME/REALTIME)
 * @state:      Flag that represents if the alarm is set to fire or not
 * @data:       Internal data value.
 */
struct txalarm {
        struct timerqueue_node  node;
        struct hrtimer          timer;
        enum alarmtimer_restart (*function)(struct txalarm *, ktime_t now);
        enum txalarmtimer_type    type;
        int                     state;
        void                    *data;
	unsigned long           timer_expiration_ns;
	unsigned long   	sum;
};


/**
 * struct txalarm_base - Alarm timer bases
 * @lock:		Lock for syncrhonized access to the base
 * @timerqueue:		Timerqueue head managing the list of events
 * @timer: 		hrtimer used to schedule events while running
 * @gettime:		Function to read the time correlating to the base
 * @base_clockid:	clockid for the base
 */
static struct txalarm_base {
	spinlock_t		lock;
	struct timerqueue_head	timerqueue;
	ktime_t			(*gettime)(void);
	clockid_t		base_clockid;
	int 			count;
	unsigned long   	sum;
} txalarm_bases[1];

/**
 * txalarmtimer_enqueue - Adds an alarm timer to an alarm_base timerqueue
 * @base: pointer to the base where the timer is being run
 * @alarm: pointer to alarm being enqueued.
 *
 * Adds alarm to a txalarm_base timerqueue
 *
 * Must hold base->lock when calling.
 */
static void txalarmtimer_enqueue(struct txalarm_base *base, struct txalarm *alarm)
{
	if (alarm->state & ALARMTIMER_STATE_ENQUEUED)
	{
		timerqueue_del(&base->timerqueue, &alarm->node);
	}

	timerqueue_add(&base->timerqueue, &alarm->node);
	alarm->state |= ALARMTIMER_STATE_ENQUEUED;
	base->count++;
}

/**
 * txalarmtimer_remove - Removes an alarm timer from an txalarm_base timerqueue
 * @base: pointer to the base where the timer is running
 * @alarm: pointer to alarm being removed
 *
 * Removes alarm to a txalarm_base timerqueue
 *
 * Must hold base->lock when calling.
 */
static void txalarmtimer_remove(struct txalarm_base *base, struct txalarm *alarm)
{
	if (!(alarm->state & ALARMTIMER_STATE_ENQUEUED))
	{
		return;
	}
	
	timerqueue_del(&base->timerqueue, &alarm->node);
	
	alarm->state &= ~ALARMTIMER_STATE_ENQUEUED;
	if (base->count>=1)
	  base->count--;
	if (base->sum >0)
	 base->sum = base->sum - alarm->timer_expiration_ns;
}


/**
 * txalarmtimer_fired - Handles txalarm hrtimer being fired.
 * @timer: pointer to hrtimer being run
 *
 * When a txalarm timer fires, this runs through the timerqueue to
 * see which alarms expired, and runs those. If there are more alarm
 * timers queued for the future, we set the hrtimer to fire when
 * when the next future alarm timer expires.
 */
static enum hrtimer_restart txalarmtimer_fired(struct hrtimer *timer)
{
	struct txalarm *alarm = container_of(timer, struct txalarm, timer);
	struct txalarm_base *base = &txalarm_bases[alarm->type];
	unsigned long flags;
	int ret = HRTIMER_NORESTART;
	int restart = ALARMTIMER_NORESTART;
	
#ifdef BCM_TTY_DEBUG	
	printk( KERN_INFO "[GPSTTYCHAR]: txalarmtimer_fired\n");
#endif	

	spin_lock_irqsave(&base->lock, flags);
	txalarmtimer_remove(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);

	if (alarm->function)
	{
		restart = alarm->function(alarm, base->gettime());
	}

#ifdef USE_HRTIMER_RESTART	
	spin_lock_irqsave(&base->lock, flags);
	if (restart != ALARMTIMER_NORESTART) 
	{
		hrtimer_set_expires(&alarm->timer, alarm->node.expires);
		txalarmtimer_enqueue(base, alarm);
		ret = HRTIMER_RESTART;
	}
	spin_unlock_irqrestore(&base->lock, flags);
#endif	
	
	// remove memory for alarm
	if (alarm!=NULL)
	{
	  spin_lock_irqsave(&base->lock, flags);
	  kfree((struct txalarm *)alarm);
	  spin_unlock_irqrestore(&base->lock, flags);
	}

	return ret;
}

/**
 * alarm_handle_timer - underlying callback function called by txalarmtimer_fired with access to struct bcm_tty_priv 
 * @alarm: ptr to alarm to be initialized
 * @now: ktime_t time when the alarm fires
 */
static enum alarmtimer_restart alarm_handle_timer(struct txalarm *alarm,
							ktime_t now)
{
	struct bcm_tty_priv *priv = NULL;
	struct k_itimer *ptr = container_of(alarm, struct k_itimer, it.alarm.alarmtimer);
	enum alarmtimer_restart result = ALARMTIMER_NORESTART;
	unsigned long flags;
	
#ifdef BCM_TTY_DEBUG	
	printk( KERN_INFO "[GPSTTYCHAR]: alarm_handle_timer %p %p\n", ptr,alarm);
#endif	
	
	spin_lock_irqsave(&ptr->it_lock, flags);
	
	if (alarm!=NULL)
	{
	  struct txalarm_base *base = &txalarm_bases[alarm->type];
	  
	  priv = (struct bcm_tty_priv *)alarm->data;
#ifdef BCM_TTY_DEBUG	  
	  printk( KERN_INFO "[GPSTTYCHAR]: alarm_handle_timer data=%p %d %d\n", priv, base->count, base->sum);
#endif
	  
	  if (base->count == 0 && base->sum == 0)
	  {
	    bcm477x_bye(priv);
	  }
	}
	
	spin_unlock_irqrestore(&ptr->it_lock, flags);
	
	return result;
}

ktime_t txalarm_expires_remaining(const struct txalarm *alarm)
{
	struct txalarm_base *base = &txalarm_bases[alarm->type];
	return ktime_sub(alarm->node.expires, base->gettime());
}

/**
 * txalarm_init - Initialize an alarm structure
 * @alarm: ptr to alarm to be initialized
 * @type: the type of the alarm
 * @function: callback that is run when the alarm fires
 */
static void txalarm_init(struct txalarm *alarm, enum alarmtimer_type type,
		enum alarmtimer_restart (*function)(struct txalarm *, ktime_t), struct bcm_tty_priv *priv, unsigned long timer_expiration_ns)
{
	timerqueue_init(&alarm->node);
	hrtimer_init(&alarm->timer, txalarm_bases[type].base_clockid, HRTIMER_MODE_ABS);
	
	alarm->timer.function = txalarmtimer_fired;
	alarm->function = function;
	alarm->type  = type;
	alarm->state = ALARMTIMER_STATE_INACTIVE;
	alarm->data  = priv;
	alarm->timer_expiration_ns = timer_expiration_ns;
}

/**
 * txalarm_start - Sets an alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time to run the alarm
 */
static void txalarm_start(struct txalarm *alarm, ktime_t start)
{
	struct txalarm_base *base = &txalarm_bases[alarm->type];
	unsigned long flags;
	
	spin_lock_irqsave(&base->lock, flags);
	alarm->node.expires = start;
	txalarmtimer_enqueue(base, alarm);
	hrtimer_start(&alarm->timer, alarm->node.expires, HRTIMER_MODE_ABS);
	spin_unlock_irqrestore(&base->lock, flags);
}

//throughput = total bits transmitted—overhead
// 921600 baud = 921600 bits/sec
// If using 8-bit data, 1 start, 1 stop, and no parity bits, the effective 
// real throughput is: 921600 * 8 / 10 = 737280 bits/sec = 92160 bytes per second
//    92160    1 second
//        1    x second
// x second = 1/92160 second for one byte  = 0.000010851 seconds per byte = 0.010851 ms per byte = 10 ns per byte
static int timer_arm( struct bcm_tty_priv *priv, size_t bytes )
{
  ktime_t ktime;
  unsigned long timer_expiration_ns = (bytes * 10) + 10;
  struct txalarm *alarm = (struct txalarm *)kmalloc(sizeof(struct txalarm), GFP_KERNEL); 
  enum  alarmtimer_type type = TXALARM_REALTIME;
  struct txalarm_base *base = NULL;
  unsigned long flags;
  
  txalarm_init(alarm, type, alarm_handle_timer, priv, timer_expiration_ns);
  
  base = &txalarm_bases[alarm->type];
  spin_lock_irqsave(&base->lock, flags);
  base->sum += timer_expiration_ns;
  ktime = ktime_set( 0,  base->sum);
  
#ifdef BCM_TTY_DEBUG  
  printk(KERN_INFO "[GPSTTYCHAR]: timer_arm %d bytes in %d ns\n", bytes, base->sum);
#endif  

  spin_unlock_irqrestore(&base->lock, flags);
  
  txalarm_start(alarm, ktime);
 
  return 0;
}


/*
 * bcm_tty_config - UART hardware settings
 */
static int bcm_tty_config(struct file *f)
{
	struct termios termios;
	mm_segment_t fs;
	long ret;

	/* Change address limit */
	fs = get_fs();
	set_fs(KERNEL_DS);

	/* Get termios */
	ret = f->f_op->unlocked_ioctl(f, TCGETS, (unsigned long)&termios);
	if (ret) {
		pr_err("%s TCGETS failed, err=%ld\n",__func__,ret);
		return -1;
	}

	termios.c_iflag = 0;
	termios.c_oflag = 0;
	termios.c_lflag = 0;
	termios.c_cflag = CRTSCTS | CLOCAL | CS8 | CREAD;
	termios.c_cc[VMIN ] = 0;
	termios.c_cc[VTIME] = 1;	/* 100ms timeout */
	termios.c_cflag |= B921600;	//TODO: How to change baud

	/* Set termios */
	ret = f->f_op->unlocked_ioctl(f, TCSETS, (unsigned long)&termios);
	if (ret) {
		pr_err("%s TCSETS failed, err=%ld\n",__func__,ret);
		return -1;
	}

	/* Restore address limit */
	set_fs(fs);

	return 0;
}


//--------------------------------------------------------------
//
//               File Operations
//
//--------------------------------------------------------------
static int bcm_tty_open(struct inode *inode, struct file *filp)
{
	/* Initially, file->private_data points device itself and we can get our priv structs from it. */
	struct bcm_tty_priv *priv = container_of(filp->private_data, struct bcm_tty_priv, misc);

	pr_info("%s++\n", __func__);

	/* Open tty */
	priv->tty = filp_open(PORT_NAME, O_RDWR, 0);
	if (IS_ERR(priv->tty)) {
		int ret = (int)PTR_ERR(priv->tty);
		pr_err("%s can not open %s, error=%d\n", __func__, PORT_NAME, ret);
		return ret;
	}

	/* Config tty */	
	if (bcm_tty_config(priv->tty)) {
		pr_err("%s can not change %s setting.\n", __func__, PORT_NAME);
		return -EIO;
	}

	filp->private_data = priv;

	pr_info("%s--\n", __func__);
	return 0;
}

static int bcm_tty_release(struct inode *inode, struct file *filp)
{
	struct bcm_tty_priv *priv = (struct bcm_tty_priv*) filp->private_data;
	struct file *tty = priv->tty;

	return filp_close(tty, 0);
}

static ssize_t bcm_tty_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	struct bcm_tty_priv *priv = (struct bcm_tty_priv*) filp->private_data;
	struct file *tty = priv->tty;
	ssize_t len;

	len = tty->f_op->read(tty, buf, size, ppos);

	return len;
}

static ssize_t bcm_tty_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
	struct bcm_tty_priv *priv = (struct bcm_tty_priv*) filp->private_data;
	struct file *tty = priv->tty;
	ssize_t ret;

	bcm477x_hello(priv);
#ifdef USE_HRTIMER    
	timer_arm(priv, size);
#endif    
	ret = tty->f_op->write(tty, buf, size, ppos);
#ifndef USE_HRTIMER   
    bcm477x_bye(priv);
#endif
	
	return ret;
}

static unsigned int bcm_tty_poll(struct file *filp, poll_table *wait)
{
	struct bcm_tty_priv *priv = (struct bcm_tty_priv*) filp->private_data;
	struct file *tty = priv->tty;

	return tty->f_op->poll(tty, wait);
}

static long bcm_tty_ioctl( struct file *filp,
        unsigned int cmd, unsigned long arg)
{
    printk("[GPSTTYCHAR]: gpstty driver bcm_tty_ioctl is coming\n" );
    return 0;
}

static const struct file_operations bcm_tty_fops = {
	.owner          =  THIS_MODULE,
	.open           =  bcm_tty_open,
	.release        =  bcm_tty_release,
	.read           =  bcm_tty_read,
	.write          =  bcm_tty_write,
	.poll           =  bcm_tty_poll,
	.unlocked_ioctl = bcm_tty_ioctl,
};

//--------------------------------------------------------------
//
//               Module init/exit
//
//--------------------------------------------------------------
static int __init bcm_tty_init(void)
{
	struct bcm_tty_priv *priv;
	int ret, irq;
        int mcu_req = 0;
	int mcu_resp = 0;
	/* Check GPIO# */

	/*===================================================
	  We need folowing OF node in dts

	  bcm477x-gpio {
		  ssp-mcu-req = <some_gpio_number>
		  ssp-mcu-resp = <some_gpio_number>
		  ssp-host-req = <some_gpio_number>
	  }
	  ===================================================== */
	struct device_node *np = of_find_node_by_name(NULL, "gps_power");
	if (!np) {
		pr_err("[GPSTTYCHAR] fail to find OF node huawei,gps_power\n");
		goto err_exit;
	}
	mcu_req = of_get_named_gpio(np,"huawei,mcu_req",0);
	mcu_resp = of_get_named_gpio(np,"huawei,mcu_req_rsp",0);
	//host_req = of_get_named_gpio(np,"huawei,gps_hostwake",0);

	pr_info("[GPSTTYCHAR] huawei, huawei,mcu_req=%d, huawei,mcu_req_rsp=%d\n",
			mcu_req, mcu_resp);
       if (mcu_req<0 || mcu_resp<0) {
		pr_err("[GPSTTYCHAR]: GPIO value not correct\n");
		goto err_exit;
	}

	/* Config GPIO */
	gpio_request(mcu_req, "MCU REQ");
	gpio_direction_output(mcu_req, 0);
	gpio_request(mcu_resp, "MCU RESP");
	gpio_direction_input(mcu_resp);

	/* Alloc */
	priv = (struct bcm_tty_priv*) kmalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		pr_err("%s Failed to allocate \"gpstty\"\n");
		goto err_exit;
	}

	memset(priv, 0, sizeof(*priv));

	/* Init - gpios */
	//priv->host_req = host_req;
	priv->mcu_req  = mcu_req;
	priv->mcu_resp = mcu_resp;

	/* Register misc device */
	priv->misc.minor = MISC_DYNAMIC_MINOR;
	priv->misc.name = "gpstty";
	priv->misc.fops = &bcm_tty_fops;

	ret = misc_register(&priv->misc);
	if (ret) {
		pr_err("%s Failed to register \"gpstty\". err=%d\n", __func__,ret);
		goto free_mem;
	}
	
	 /* Initialize alarm bases */
#ifdef USE_HRTIMER
	 txalarm_bases[TXALARM_REALTIME].base_clockid = CLOCK_REALTIME;
	 txalarm_bases[TXALARM_REALTIME].gettime = &ktime_get_real;
	 txalarm_bases[TXALARM_REALTIME].count = 0;
	 txalarm_bases[TXALARM_REALTIME].sum = 0;
	 timerqueue_init_head(&txalarm_bases[TXALARM_REALTIME].timerqueue);
	 spin_lock_init(&txalarm_bases[TXALARM_REALTIME].lock);
#endif     


	return 0;

free_mem:
	if (priv) 
		kfree(priv);
err_exit:
	return -ENODEV;
}

static void __exit bcm_tty_exit(void)
{
}

module_init(bcm_tty_init);
module_exit(bcm_tty_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BCM TTY Driver");

