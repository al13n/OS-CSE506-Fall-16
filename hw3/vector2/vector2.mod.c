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
	{ 0x1dcd08a6, __VMLINUX_SYMBOL_STR(unregister_vector) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x94339772, __VMLINUX_SYMBOL_STR(register_vector) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xc671e369, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xb5419b40, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xb8cb487b, __VMLINUX_SYMBOL_STR(original_syscall) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xcb7f5083, __VMLINUX_SYMBOL_STR(d_path) },
	{ 0xc63e3f40, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xfbef6fea, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x52d11933, __VMLINUX_SYMBOL_STR(path_get) },
	{ 0x5279abff, __VMLINUX_SYMBOL_STR(fget) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=hook_syscall_table";


MODULE_INFO(srcversion, "921B160EC29E38E3893786D");
