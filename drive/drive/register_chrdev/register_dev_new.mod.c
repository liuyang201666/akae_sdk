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
	{ 0xa5423cc4, "param_get_int" },
	{ 0xcb32da10, "param_set_int" },
	{ 0x98082893, "__copy_to_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x17386eae, "cdev_add" },
	{ 0x8abf2934, "cdev_init" },
	{ 0x1d1355a7, "cdev_alloc" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xea147363, "printk" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x4508a474, "cdev_del" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";
