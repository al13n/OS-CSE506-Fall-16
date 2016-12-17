#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "../ioctls/ioctls.h"
#include "../ioctls_s.h"

int main(int argc, char **argv) {
        char device[] = "/dev/ioctls";
        int fd = open(device, 0);
	int ret = ioctl(fd, IOCTL_CHANGE_VECTOR, 2);
	mkdir("v1directory", 0700);
	char *buf1 = (char *)malloc(5000);
	ret = ioctl(fd, IOCTL_LIST_ALL, buf1);
	printf("%s\n", buf1);
	struct ioctls_s buf;
	buf.buf = (char *)malloc(4096);
	buf.pid = getpid();
        ret = ioctl(fd, IOCTL_PROC_VECTOR, &buf);
	printf("Vector ID: %s\n", buf.buf);
	int write_protected_file = open("hello.protected", O_CREAT | O_WRONLY, S_IRUSR);
	char buf3[6] = "hello\0";
	int write_bytes = write(write_protected_file, buf3, strlen(buf3) + 1);
	char *buf2 = (char *)malloc(255);
	int read_protected_file = open("hello.protected", O_RDONLY, S_IRUSR);
	
	int read_bytes = read(read_protected_file, buf2, 255);
	printf("READ: %s\n", buf2);
	ret = ioctl(fd, IOCTL_CHANGE_VECTOR, 0);
	return 0;
}
