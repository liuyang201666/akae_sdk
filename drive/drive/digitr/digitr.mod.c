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
	{ 0xfcec0987, "enable_irq" },
	{ 0xb6c70a7d, "__wake_up" },
	{ 0xe821b37, "register_chrdev" },
	{ 0x2cf190e3, "request_irq" },
	{ 0xde75b689, "set_irq_type" },
	{ 0xea147363, "printk" },
	{ 0x9ef749e2, "unregister_chrdev" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xbb72d4fe, "__put_user_1" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x51493d94, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8085c7b1, "prepare_to_wait" },
	{ 0x5f754e5a, "memset" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

