#ifndef __IS_VECTOR_TABLE__
#define __IS_VECTOR_TABLE__
#endif
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/syscalls.h>
#include<linux/kobject.h>
#include<linux/slab.h>
#include<linux/cred.h>
#include<linux/namei.h>
#include<linux/path.h>
#include<linux/file.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<asm/segment.h>
#include<asm/uaccess.h>
#include<linux/buffer_head.h>
#include "../sys_ops.h"

MODULE_LICENSE("GPL");

struct file* logfilp = NULL;
loff_t pos = 0;

int file_write(
	struct file* file,
	unsigned char* data,
	unsigned int size)
{
	mm_segment_t oldfs;
	int ret;
	
	oldfs = get_fs();
	set_fs(get_ds());
	ret = vfs_write(file, data, size, &pos);
	set_fs(oldfs);
	pos += size;
	return ret;
}

int my_mkdir(const char *pathname, mode_t mode) {
	char *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = original_syscall.mkdir(pathname, mode);
	sprintf(tmp, "OP: mkdir(%s, %d)=%d\n", pathname, mode, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;
}

int my_rmdir(const char *pathname)
{
	char *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = original_syscall.rmdir(pathname);
	sprintf(tmp, "OP: rmdir(%s)=%d\n", pathname, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;
}

int my_symlink(const char *target, const char *linkpath)
{
	char *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = original_syscall.symlink(target, linkpath);
	sprintf(tmp, "OP: symlink(%s, %s)=%d\n", target, linkpath, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;
}

int my_unlink(const char *pathname)
{
	int ret;
	char *tmp1, *tmp;
	tmp1 = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	ret = copy_from_user((void *)tmp1, (const void *)pathname, strlen_user(pathname));
	ret = original_syscall.unlink(pathname);
	sprintf(tmp, "OP: unlink(%s)=%d\n", tmp1, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;
}

int my_open(const char *pathname, int flags, mode_t mode)
{
	char *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = original_syscall.open(pathname, flags, mode);
	sprintf(tmp, "OP: open(%s, %d, %d)=%d\n", pathname, flags, mode, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;
}

int my_close(int fd)
{
	char *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = original_syscall.close(fd);
	sprintf(tmp, "OP: close(%d)=%d\n", fd, ret);
	file_write(logfilp, tmp, strlen(tmp)+1);
	kfree(tmp);
	return ret;	
}

int vector1_init(void)
{
	/* register vector1 module */
	struct sys_ops ops;
	int ret = 0;
	mm_segment_t oldfs;

	ops.mkdir = my_mkdir;
	ops.read = NULL;
	ops.write = NULL;
	ops.open = my_open;
	ops.close = my_close;
	ops.rmdir = my_rmdir;
	ops.setuid = NULL;
	ops.getdents = NULL;
	ops.unlink = my_unlink;
	ops.symlink = my_symlink;

	ret = register_vector(THIS_MODULE, ops);
	
	oldfs = get_fs();
	set_fs(get_ds());
	logfilp = filp_open("/tmp/tmplogs", O_WRONLY|O_APPEND|O_CREAT, 0644);
	set_fs(oldfs);
	return 0;
}

void vector1_exit(void)
{
	if (unregister_vector(THIS_MODULE) == 0)
		filp_close(logfilp, NULL);
}

module_init(vector1_init);
module_exit(vector1_exit);
