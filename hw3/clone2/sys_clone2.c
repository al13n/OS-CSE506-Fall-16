#include <linux/linkage.h>
#include <linux/moduleloader.h>

extern int increment_ref_count(int);
extern long _do_fork(unsigned long clone_flags, unsigned long stack_start,
		     unsigned long stack_size, int __user *parent_tidptr,
		     int __user *child_tidptr, unsigned long tls,
		     int vector_id);

asmlinkage extern long (*sysptr) (unsigned long clone_flags,
				  unsigned long newsp,
				  int __user *parent_tidptr, unsigned long tls,
				  int __user *child_tidptr, int vector_id);

asmlinkage long clone2(unsigned long clone_flags, unsigned long newsp,
		       int __user *parent_tidptr, unsigned long tls,
		       int __user *child_tidptr, int vector_id)
{
	long ret = 0;
	ret = increment_ref_count(vector_id);
	if (ret < 0)
		return -EINVAL;
	ret = _do_fork(clone_flags, 0, 0, NULL, NULL, 0, vector_id);
	printk("ret_val from do fork is:%lu\n", ret);
	return ret;
}

static int __init init_sys_clone2(void)
{
	printk("installed new sys_clone2 module\n");
	if (sysptr == NULL)
		sysptr = clone2;
	return 0;
}

static void __exit exit_sys_clone2(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	printk("removed sys_clone2 module\n");
}

module_init(init_sys_clone2);
module_exit(exit_sys_clone2);
MODULE_LICENSE("GPL");
