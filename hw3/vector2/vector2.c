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
#include "../sys_ops.h"
#define MAGIC_GROUP_NUMBER 1337
#define CIPHER 1337
MODULE_LICENSE("GPL");

int endsWith(const char *str, const char *suffix)
{
	ssize_t lenstr, lensuffix;

	if (!str || !suffix)
		return 0;
	lenstr = strlen(str);
	lensuffix = strlen(suffix);
	if (lensuffix >  lenstr)
		return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

ssize_t my_read(int fd, void *buf, size_t count)
{
	char *tmp, *tmp1;
	int i, ret;
	ssize_t read_bytes;
	struct file *fp = NULL;
	struct path file_path;
	char *path;

	fp = fget(fd);
	file_path = fp->f_path;
	path_get(&fp->f_path);
	tmp1 = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	path = d_path(&file_path, tmp1, PAGE_SIZE);
	kfree(tmp1);
	read_bytes = original_syscall.read(fd, buf, count);
	if (endsWith(path, ".protected")) {
		tmp = (char *)kmalloc(read_bytes, GFP_KERNEL);
		ret = copy_from_user(
			(void *)tmp,
			(const void *)buf,
			read_bytes);
		for (i = 0; i < read_bytes; i++)
			*(tmp+i) = *(tmp+i)^CIPHER;
		ret = copy_to_user((void *)buf, (const void *)tmp, read_bytes);
		kfree(tmp);
	}
	return read_bytes;
}

ssize_t my_write(int fd, const void *buf, size_t count)
{
	char *tmp, *tmp1;
	int i, ret;
	ssize_t write_bytes;
	struct file *fp = NULL;
	struct path file_path;
	char *path;

	fp = fget(fd);
	file_path = fp->f_path;
	path_get(&fp->f_path);
	tmp1 = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	path = d_path(&file_path, tmp1, PAGE_SIZE);
	write_bytes = 0;
	if (endsWith(path, ".protected")) {
		tmp = (char *)kmalloc(count, GFP_KERNEL);
		ret = copy_from_user((void *)tmp, (const void *)buf, count);
		strncpy(tmp1, tmp, count);
		for (i = 0; i < count; i++)
			*(tmp+i) = *(tmp+i)^CIPHER;
		ret = copy_to_user((void *)buf, (const void *)tmp, count);
		write_bytes = original_syscall.write(fd, buf, count);
		ret = copy_to_user((void *)buf, (const void *)tmp1, count);
		kfree(tmp);
	} else
		write_bytes = original_syscall.write(fd, buf, count);
	kfree(tmp1);
	return write_bytes;
}

int vector2_init(void)
{
	struct sys_ops ops;
	int ret;

	ops.mkdir = NULL;
	ops.read = my_read;
	ops.write = my_write;
	ops.open = NULL;
	ops.close = NULL;
	ops.rmdir = NULL;
	ops.setuid = NULL;
	ops.getdents = NULL;
	ops.unlink = NULL;
	ops.symlink = NULL;

	ret = register_vector(THIS_MODULE, ops);
	if (ret < 0)
		printk(KERN_INFO"Cannot register vector\n");
	return 0;
}

void vector2_exit(void)
{
	unregister_vector(THIS_MODULE);
}

module_init(vector2_init);
module_exit(vector2_exit);
