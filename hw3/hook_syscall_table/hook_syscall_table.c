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
#include<linux/list.h>
#include "vector_systable.h"

MODULE_LICENSE("GPL");

#define START_SYSCALL 0xffffffff81000000
#define END_SYSCALL   0xffffffffa2000000

unsigned long *system_call_table_array;
struct mutex vector_list_lock;
struct vector_systable registered_vectors;
struct sys_ops original_syscall;
EXPORT_SYMBOL(original_syscall);

#ifndef __MY_OPEN_SYSCALL__
#define __MY_OPEN_SYSCALL__

asmlinkage ssize_t my_open(const char *pathname, int flags, mode_t mode)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.open != NULL))
			return tmp1->ops.open(pathname, flags, mode);
	}

	return original_syscall.open(pathname, flags, mode);
}

#endif

#ifndef __MY_CLOSE_SYSCALL__
#define __MY_CLOSE_SYSCALL__

asmlinkage ssize_t my_close(int fd)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.close != NULL))
			return tmp1->ops.close(fd);
	}

	return original_syscall.close(fd);
}

#endif


#ifndef __MY_RMDIR_SYSCALL__
#define __MY_RMDIR_SYSCALL__

asmlinkage int my_rmdir(const char *pathname)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.rmdir != NULL))
			return tmp1->ops.rmdir(pathname);
	}

	return original_syscall.rmdir(pathname);
}

#endif


#ifndef __MY_MKDIR_SYSCALL__
#define __MY_MKDIR_SYSCALL__

asmlinkage int my_mkdir(const char *pathname, mode_t mode)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.mkdir != NULL))
			return tmp1->ops.mkdir(pathname, mode);
	}

	return original_syscall.mkdir(pathname, mode);
}

#endif

#ifndef __MY_READ_SYSCALL__
#define __MY_READ_SYSCALL__

asmlinkage int my_read(int fd, void *buf, size_t count)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.read != NULL))
			return tmp1->ops.read(fd, buf, count);
	}

	return original_syscall.read(fd, buf, count);
}

#endif

#ifndef __MY_WRITE_SYSCALL__
#define __MY_WRITE_SYSCALL__

asmlinkage int my_write(int fd, const void *buf, size_t count)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (current->vector_id == tmp1->id && (tmp1->ops.write != NULL))
			return tmp1->ops.write(fd, buf, count);
	}

	return original_syscall.write(fd, buf, count);
}

#endif

#ifndef __MY_GETDENTS_SYSCALL__
#define __MY_GETDENTS_SYSCALL__

asmlinkage int my_getdents(
	unsigned int fd,
	struct linux_dirent *dirp,
	unsigned int count
	)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (
			current->vector_id == tmp1->id
			&& (tmp1->ops.getdents != NULL)
		) {
			return tmp1->ops.getdents(fd, dirp, count);
		}
	}

	return original_syscall.getdents(fd, dirp, count);
}

#endif

#ifndef __MY_SETUID_SYSCALL__
#define __MY_SETUID_SYSCALL__

asmlinkage int my_setuid(uid_t uid)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (
			current->vector_id == tmp1->id
			&& (tmp1->ops.setuid != NULL)
		)
			return tmp1->ops.setuid(uid);
	}

	return original_syscall.setuid(uid);
}

#endif

#ifndef __MY_SYMLINK_SYSCALL__
#define __MY_SYMLINK_SYSCALL__

asmlinkage int my_symlink(const char *path1, const char *path2)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (
			current->vector_id == tmp1->id
			&& (tmp1->ops.symlink != NULL)
		)
			return tmp1->ops.symlink(path1, path2);
	}

	return original_syscall.symlink(path1, path2);
}

#endif

#ifndef __MY_UNLINK_SYSCALL__
#define __MY_UNLINK_SYSCALL__

asmlinkage int my_unlink(const char *path)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (
			current->vector_id == tmp1->id
			&& (tmp1->ops.unlink != NULL)
		)
			return tmp1->ops.unlink(path);
	}

	return original_syscall.unlink(path);
}

#endif

/*function to disable control register write protection.*/
void disable_write_protection(void)
{
	unsigned long cr0, bit_16;

	bit_16 = 0x10000;
	cr0 = read_cr0() & (~bit_16);
	write_cr0(cr0);
}

/*function to enable control register write protection.*/
void enable_write_protection(void)
{
	unsigned long cr0, bit_16;

	bit_16 = 0x10000;
	cr0 = read_cr0() | bit_16;
	write_cr0(cr0);
}

/*function to find the system call table starting address in memory.*/
unsigned long **find_syscall_table(void)
{
	unsigned long **system_call_table_array;
	unsigned long i = START_SYSCALL;

	while (i < END_SYSCALL) {
		system_call_table_array = (unsigned long **)i;
		if (system_call_table_array[__NR_close] ==
		    (unsigned long *)sys_close)
			return &system_call_table_array[0];
		i = i + sizeof(void *);
	}
	return NULL;
}

int unregister_vector(struct module *vector_module)
{
	struct vector_systable *tmp;

	list_for_each_entry(tmp, &registered_vectors.vector_list, vector_list) {
		if (vector_module == tmp->vector_module) {
			if (tmp->ref_cnt == 0)
				list_del(&tmp->vector_list);
			else
				return -1;
			break;
		}
	}
	return 0;
}
EXPORT_SYMBOL(unregister_vector);

int list_all_vectors(char __user *buf)
{
	struct vector_systable *tmp1;
	char *tmp;
	int len;
	int ret;

	mutex_lock(&vector_list_lock);
	tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	len = ret = 0;
	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list
	) {
		sprintf(
			tmp+len,
			"ID: %d, Name: %s\n",
			tmp1->id,
			tmp1->vector_module->name
		);
		len += strlen(tmp1->vector_module->name) + 14;
	}
	ret = copy_to_user(buf, tmp, len) ? -EFAULT : 0;
	kfree(tmp);
	mutex_unlock(&vector_list_lock);

	return ret;
}
EXPORT_SYMBOL(list_all_vectors);

int _reduce_ref_count(int vector_id)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list){
		if (tmp1->id == vector_id) {
			module_put(tmp1->vector_module);
			tmp1->ref_cnt = tmp1->ref_cnt - 1;
			return 0;
		}
	}
	return -EINVAL;
}

int _increment_ref_count(int vector_id)
{
	struct vector_systable *tmp1;

	list_for_each_entry(
		tmp1,
		&registered_vectors.vector_list,
		vector_list) {
		if (tmp1->id == vector_id) {
			try_module_get(tmp1->vector_module);
			tmp1->ref_cnt = tmp1->ref_cnt + 1;
			return 0;
		}
	}
	return -EINVAL;
}

int reduce_ref_count(int vector_id)
{
	int ret;

	mutex_lock(&vector_list_lock);
	ret = _reduce_ref_count(vector_id);
	mutex_unlock(&vector_list_lock);
	return ret;
}
EXPORT_SYMBOL(reduce_ref_count);

int increment_ref_count(int vector_id)
{
	int ret;

	mutex_lock(&vector_list_lock);
	ret = _increment_ref_count(vector_id);
	mutex_unlock(&vector_list_lock);
	return ret;
}
EXPORT_SYMBOL(increment_ref_count);

int update_process_vector_id(int new_vector_id)
{
	int old_vector_id = current->vector_id;

	mutex_lock(&vector_list_lock);
	if (old_vector_id != 0 && _reduce_ref_count(old_vector_id) < 0)
		return -EINVAL;
	if (new_vector_id != 0 && _increment_ref_count(new_vector_id) < 0)
		return -EINVAL;
	current->vector_id = new_vector_id;
	mutex_unlock(&vector_list_lock);
	return 0;
}
EXPORT_SYMBOL(update_process_vector_id);

int register_vector(struct module *vector_module, struct sys_ops ops)
{
	struct vector_systable *tmp;
	static int id = 1;

	mutex_lock(&vector_list_lock);
	tmp = (struct vector_systable *)kmalloc(
		sizeof(struct vector_systable),
		GFP_KERNEL);
	tmp->id = id++;
	tmp->ref_cnt = 0;
	tmp->vector_module = vector_module;
	tmp->ops = ops;
	list_add(&(tmp->vector_list), &(registered_vectors.vector_list));
	mutex_unlock(&vector_list_lock);
	return 0;
}
EXPORT_SYMBOL(register_vector);

int hook_syscall_table_init(void)
{
	system_call_table_array = (unsigned long *)find_syscall_table();
	if (!system_call_table_array)
		printk(KERN_INFO"syscall table not found\n");

	disable_write_protection();

	#ifdef __MY_READ_SYSCALL__
	original_syscall.read  = (void *)xchg(
		&system_call_table_array[__NR_read],
		(unsigned long)my_read);
	#endif

	#ifdef __MY_WRITE_SYSCALL__
	original_syscall.write = (void *)xchg(
		&system_call_table_array[__NR_write],
		(unsigned long)my_write);
	#endif

	#ifdef __MY_MKDIR_SYSCALL__
	original_syscall.mkdir = (void *)xchg(
		&system_call_table_array[__NR_mkdir],
		(unsigned long)my_mkdir);
	#endif

	#ifdef __MY_RMDIR_SYSCALL__
	original_syscall.rmdir = (void *)xchg(
		&system_call_table_array[__NR_rmdir],
		(unsigned long)my_rmdir);
	#endif

	#ifdef __MY_OPEN_SYSCALL__
	original_syscall.open = (void *)xchg(
		&system_call_table_array[__NR_open],
		(unsigned long)my_open);
	#endif

	#ifdef __MY_CLOSE_SYSCALL__
	original_syscall.close = (void *)xchg(
		&system_call_table_array[__NR_close],
		(unsigned long)my_close);
	#endif

	#ifdef __MY_GETDENTS_SYSCALL__
	original_syscall.getdents = (void *)xchg(
		&system_call_table_array[__NR_getdents],
		(unsigned long)my_getdents);
	#endif

	#ifdef __MY_SETUID_SYSCALL__
	original_syscall.setuid = (void *)xchg(
		&system_call_table_array[__NR_setuid],
		(unsigned long)my_setuid);
	#endif

	#ifdef __MY_SYMLINK_SYSCALL__
	original_syscall.symlink = (void *)xchg(
		&system_call_table_array[__NR_symlink],
		(unsigned long)my_symlink);
	#endif

	#ifdef __MY_UNLINK_SYSCALL__
	original_syscall.unlink = (void *)xchg(
		&system_call_table_array[__NR_unlink],
		(unsigned long)my_unlink);
	#endif


	enable_write_protection();

	INIT_LIST_HEAD(&registered_vectors.vector_list);
	mutex_init(&vector_list_lock);

	return 0;
}

void hook_syscall_table_exit(void)
{
	disable_write_protection();

	#ifdef __MY_READ_SYSCALL__
	system_call_table_array[__NR_read] =
		(unsigned long)original_syscall.read;
	#endif

	#ifdef __MY_WRITE_SYSCALL__
	system_call_table_array[__NR_write] =
		(unsigned long)original_syscall.write;
	#endif

	#ifdef __MY_MKDIR_SYSCALL__
	system_call_table_array[__NR_mkdir] =
		(unsigned long)original_syscall.mkdir;
	#endif

	#ifdef __MY_RMDIR_SYSCALL__
	system_call_table_array[__NR_rmdir] =
		(unsigned long)original_syscall.rmdir;
	#endif

	#ifdef __MY_OPEN_SYSCALL__
	system_call_table_array[__NR_open] =
		(unsigned long)original_syscall.open;
	#endif

	#ifdef __MY_CLOSE_SYSCALL__
	system_call_table_array[__NR_close] =
		(unsigned long)original_syscall.close;
	#endif

	#ifdef __MY_GETDENTS_SYSCALL__
	system_call_table_array[__NR_getdents] =
		(unsigned long)original_syscall.getdents;
	#endif

	#ifdef __MY_SETUID_SYSCALL__
	system_call_table_array[__NR_setuid] =
		(unsigned long)original_syscall.setuid;
	#endif

	#ifdef __MY_SYMLINK_SYSCALL__
	system_call_table_array[__NR_symlink] =
		(unsigned long)original_syscall.symlink;
	#endif

	#ifdef __MY_UNLINK_SYSCALL__
	system_call_table_array[__NR_unlink] =
		(unsigned long)original_syscall.unlink;
	#endif

	enable_write_protection();
}

module_init(hook_syscall_table_init);
module_exit(hook_syscall_table_exit);

