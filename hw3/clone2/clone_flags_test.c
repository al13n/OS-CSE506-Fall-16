#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CLONE_SYSCALLS 0x00001000
#define DEVICE_NUM 9
#define IOCTL_CHANGE_VECTOR _IOR(DEVICE_NUM, 1, char*)
#define IOCTL_LIST_ALL _IOR(DEVICE_NUM, 2, char*)
#define IOCTL_PROC_VECTOR _IOR(DEVICE_NUM, 3, char*)

struct ioctls_s {
        pid_t pid;
        char *buf;
};

static int /* Start function for cloned child */ childFunc(void *arg)
{
	printf("in child:%d\n",getpid());
	int rc = 0, fd = 0;
        struct ioctls_s buf;
        fd = open("/dev/ioctls",0);
        buf.buf = (char *)malloc(4096);
        buf.pid = getpid();
        rc = ioctl(fd, IOCTL_PROC_VECTOR, &buf);
        printf("child vector_id:%s\n", buf.buf);
	sleep(200);
	free(buf.buf);
	return 0;		/* Child terminates now */
}

#define STACK_SIZE (1024 * 1024)	/* Stack size for cloned child */

int main(int argc, char *argv[])
{
	char *stack;		/* Start of stack buffer */
	char *stackTop;		/* End of stack buffer */
	pid_t pid;

	/* Allocate stack for child */
	int rc = 0, fd = 0;
        struct ioctls_s buf;
        fd = open("/dev/ioctls",0);
        buf.buf = (char *)malloc(4096);
        buf.pid = getpid();
        rc = ioctl(fd, IOCTL_CHANGE_VECTOR, 2);
	rc = ioctl(fd, IOCTL_PROC_VECTOR, &buf);
        printf("parent vector_id:%s", buf.buf);
	stack = malloc(STACK_SIZE);
	stackTop = stack + STACK_SIZE;	/* Assume stack grows downward */
	printf("parent pid:%d\n", getpid());
	pid = clone(childFunc, stackTop, /*CLONE_SYSCALLS*/ 0,argv[1]);
	printf("clone() returned %ld\n", (long)pid);
	sleep(1);
	exit(EXIT_SUCCESS);
}
