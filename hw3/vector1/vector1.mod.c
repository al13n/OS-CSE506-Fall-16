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
	{ 0x886df2ee, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0x1dcd08a6, __VMLINUX_SYMBOL_STR(unregister_vector) },
	{ 0xf3326eff, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x94339772, __VMLINUX_SYMBOL_STR(register_vector) },
	{ 0xb5419b40, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xe67d81ba, __VMLINUX_SYMBOL_STR(strlen_user) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xb8cb487b, __VMLINUX_SYMBOL_STR(original_syscall) },
	{ 0xc63e3f40, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xfbef6fea, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x301fae5, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0x122e6e41, __VMLINUX_SYMBOL_STR(cpu_tss) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=hook_syscall_table";


MODULE_INFO(srcversion, "8E8D41630496AF1F907A9F0");
