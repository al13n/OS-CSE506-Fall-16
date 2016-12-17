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
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0xd00f6d32, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xde9eec8, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x6e3e9920, __VMLINUX_SYMBOL_STR(list_all_vectors) },
	{ 0xc671e369, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xc63e3f40, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xfbef6fea, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xe51b15a3, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0x1d9855e0, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0xd935bfb7, __VMLINUX_SYMBOL_STR(increment_ref_count) },
	{ 0x60b2bb11, __VMLINUX_SYMBOL_STR(reduce_ref_count) },
	{ 0x7a53d0e3, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x128d5245, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x539b82d4, __VMLINUX_SYMBOL_STR(mutex_lock) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=hook_syscall_table";


MODULE_INFO(srcversion, "1969DE10CF55C8F4AECEE62");
