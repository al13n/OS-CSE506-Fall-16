#ifndef __SYS_OPS__
#define __SYS_OPS__

	struct sys_ops {
		ssize_t (*read)(int, void *, size_t);
		ssize_t (*write)(int, const void *, size_t);
		int (*mkdir)(const char *, mode_t);
		int (*rmdir)(const char *);
		int (*open)(const char *, int, mode_t);
		int (*close)(int);
		int (*getdents)(
			unsigned int,
			struct linux_dirent *,
			unsigned int);
		int (*setuid)(uid_t);
		int (*unlink)(const char *);
		int (*symlink)(const char *, const char *);
	};

#ifdef __IS_VECTOR_TABLE__
	extern int register_vector(struct module*, struct sys_ops);
	extern int unregister_vector(struct module*);
	extern struct sys_ops original_syscall;
#endif

#endif
