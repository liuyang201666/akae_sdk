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
	{ 0x5934392b, "fb_register_client" },
	{ 0x77bf8cb, "malloc_sizes" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x323222ba, "mutex_unlock" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xf8fa7bcc, "device_register" },
	{ 0x8d3894f2, "_ctype" },
	{ 0x6c36a5c1, "__mutex_init" },
	{ 0xea147363, "printk" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0xb97d4c9c, "mutex_lock" },
	{ 0xdc74cc24, "kmem_cache_alloc" },
	{ 0x37a0cba, "kfree" },
	{ 0x12aee56c, "class_destroy" },
	{ 0xcc36f32e, "fb_unregister_client" },
	{ 0xd4ccaec6, "device_unregister" },
	{ 0x8842556b, "__class_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

