#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/signal.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/uaccess.h>

#include <linux/videodev.h>
#include <linux/video_encoder.h>

MODULE_DESCRIPTION("Analog Devices ADV7175 video encoder driver");
MODULE_AUTHOR("Dave Perks");
MODULE_LICENSE("GPL");



















static struct i2c_board_info ch7005c_info = {
	I2C_BOARD_INFO("ch7005c", 0xEA),      

 
};



static struct i2c_client *ch7005c_client;
static int at24cxx_dev_init(void)
{

	struct i2c_adapter *i2c_adap;    
	i2c_adap = i2c_get_adapter(0);   
	ch7005c_client = i2c_new_device(i2c_adap, &ch7005c_info);  
	ch7005c_client i2c_put_adapter(i2c_adap);
	return 0;
	
}
static void at24cxx_dev_exit(void)
{
	i2c_unregister_device(at24cxx_client);
}

module_init(at24cxx_dev_init);
module_exit(at24cxx_dev_exit);













static struct i2c_driver i2c_driver_ch7005c = {
	.driver = {
		.name = "ch7005c",	/* name */
	},

	.id = I2C_DRIVERID_ADV7175,

	.attach_adapter = adv7175_attach_adapter,
	.detach_client = adv7175_detach_client,
	.command = adv7175_command,
};

static int __init adv7175_init (void)
{
	return i2c_add_driver(&i2c_driver_ch7005c);
}

static void __exit
adv7175_exit (void)
{
	i2c_del_driver(&i2c_driver_adv7175);
}

module_init(adv7175_init);
module_exit(adv7175_exit);

