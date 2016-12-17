#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE_NUM 9
#define IOCTL_CHANGE_VECTOR _IOR(DEVICE_NUM, 1, char*)
#define IOCTL_LIST_ALL _IOR(DEVICE_NUM, 2, char*)
#define IOCTL_PROC_VECTOR _IOR(DEVICE_NUM, 3, char*)

#ifndef __NR_clone2
#error clone2 system call not defined
#endif

struct ioctls_s {
        pid_t pid;
        char *buf;
};

int main(int argc, const char *argv[])
{
	int rc = 0, fd = 0;
	struct ioctls_s buf;
	fd = open("/dev/ioctls",0);
	buf.buf = (char *)malloc(4096);
	buf.pid = getpid();
	rc = ioctl(fd, IOCTL_PROC_VECTOR, &buf);
	printf("%s\n", buf.buf);
  	rc = syscall(__NR_clone2, SIGCHLD, 0, NULL, 0, NULL, atoi(argv[1]));
	if (rc) {
		struct ioctls_s buf;
                fd = open("/dev/ioctls",0);
                buf.buf = (char *)malloc(4096);
                buf.pid = rc;
                ioctl(fd, IOCTL_PROC_VECTOR, &buf);
                printf("inside parent:\nchild pid:%d, vector_id:%s\n", rc, buf.buf);
		sleep(10);
		free(buf.buf);
		if (fd>0)
			close(fd);
		exit(0);		
	}
	else {
        	printf("inside child. do something\n");
		sleep(5);
		exit(rc);
	}
}
