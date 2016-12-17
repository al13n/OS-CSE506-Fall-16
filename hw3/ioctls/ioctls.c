#ifndef __IOCTLS_C__
#define __IOCTLS_C__
#endif
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include "ioctls.h"
#include "../ioctls_s.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSE506P35");
MODULE_DESCRIPTION("ioctl handling module");

struct mutex ioctl_lock;

long ioctls_handler(
	struct file *file,
	unsigned int ioctl_num,
	unsigned long buf)
{
	unsigned long ret = 0;
	int old_vector_id;
	char *tmp = NULL;
	struct task_struct *proc_struct;

	mutex_lock(&ioctl_lock);

	switch (ioctl_num) {
	case IOCTL_CHANGE_VECTOR:
		ret = access_ok(VERIFY_READ, buf, sizeof((char *)buf));
		if (ret < 0) {
			ret = -EINVAL;
			goto out;
		}
		old_vector_id  = current->vector_id;
		ret = reduce_ref_count(old_vector_id);
		if (ret < 0) {
			ret = -EINVAL;
			goto out;
		}
		ret = increment_ref_count((int)buf);
		if (ret < 0) {
			ret = -EINVAL;
			goto out;
		}
		current->vector_id = (int)buf;
		break;
	case IOCTL_LIST_ALL:
		ret = list_all_vectors((char *)buf);
		if (ret < 0) {
			ret = -EAGAIN;
			goto out;
		}
		break;
	case IOCTL_PROC_VECTOR:
		proc_struct = pid_task(
			find_vpid((pid_t)(((struct ioctls_s *)buf)->pid)),
			PIDTYPE_PID);
		if (!proc_struct) {
			ret = -ESRCH;
			goto out;
		}
		tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (!tmp) {
			ret = -ENOMEM;
			goto out;
		}
		sprintf(tmp, "%d\n", proc_struct->vector_id);
		ret = copy_to_user(((struct ioctls_s *)buf)->buf, tmp, 3);
		if (ret != 0) {
			ret = -EFAULT;
			goto out;
		}
		break;
	default:
		ret = -1;
		break;
	}
out:
	kfree(tmp);
	mutex_unlock(&ioctl_lock);
	return ret;
}

const struct file_operations ops = {
	.unlocked_ioctl = ioctls_handler,
};

static int __init ioctls_init(void)
{
	int ret;

	ret = register_chrdev(DEVICE_NUM, DEVICE_NAME, &ops);
	if (ret <  0) {
		printk(KERN_WARNING"registration failed.\n");
		goto out;
	}
	mutex_init(&ioctl_lock);
out:
	return ret;
}

static void __exit ioctls_exit(void)
{
	unregister_chrdev(DEVICE_NUM, DEVICE_NAME);
}

module_init(ioctls_init);
module_exit(ioctls_exit);
