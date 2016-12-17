#!/bin/bash
sh rm_all_mods.sh
insmod hook_syscall_table/hook_syscall_table.ko
sh ioctls/install_module.sh
insmod vector1/vector1.ko
insmod vector2/vector2.ko
insmod vector3/vector3.ko
insmod clone2/sys_clone2.ko
