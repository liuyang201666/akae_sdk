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
	{ 0x7d11c268, "jiffies" },
	{ 0x61593b72, "__mod_timer" },
	{ 0x366bdeaa, "init_timer" },
	{ 0xea147363, "printk" },
	{ 0x7e531cde, "del_timer" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";
