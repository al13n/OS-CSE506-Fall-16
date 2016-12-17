#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0xc8a5df6b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xfbef6fea, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x128d5245, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xc671e369, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x7a53d0e3, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xd00f6d32, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x539b82d4, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x9fcd92b7, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0xc63e3f40, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x773dfecd, __VMLINUX_SYMBOL_STR(pv_cpu_ops) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xddd14fd8, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0xdcb0349b, __VMLINUX_SYMBOL_STR(sys_close) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7B1CD0F903ED2FA94CA0F2D");
