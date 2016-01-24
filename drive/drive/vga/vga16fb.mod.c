#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x1d426304, "struct_module" },
	{ 0xb974e844, "platform_device_put" },
	{ 0xfc731538, "platform_device_add" },
	{ 0x1fd9d8b6, "platform_device_alloc" },
	{ 0x9ba15e83, "platform_driver_register" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x323222ba, "mutex_unlock" },
	{ 0xb97d4c9c, "mutex_lock" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x532ead47, "cfb_fillrect" },
	{ 0x578aee0d, "cfb_copyarea" },
	{ 0xa7649011, "cfb_imageblit" },
	{ 0xb969142c, "register_framebuffer" },
	{ 0x7a890c8, "fb_alloc_cmap" },
	{ 0x9d669763, "memcpy" },
	{ 0x6c36a5c1, "__mutex_init" },
	{ 0x5de39feb, "framebuffer_alloc" },
	{ 0xea147363, "printk" },
	{ 0x52b967ad, "framebuffer_release" },
	{ 0x98b71c6, "fb_dealloc_cmap" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0xaff0af7f, "unregister_framebuffer" },
	{ 0x52731d29, "platform_driver_unregister" },
	{ 0x2912b497, "platform_device_unregister" },
	{ 0xe707d823, "__aeabi_uidiv" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

