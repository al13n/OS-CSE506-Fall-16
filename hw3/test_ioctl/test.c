#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include "../ioctls_s.h"

#define IOCTL_LIST_ALL _IOR(9, 2, char*)
#define IOCTL_CHANGE_VECTOR _IOR(9, 1, char*)

int main(int argc, char **argv) {
	char device[] = "/dev/ioctls";
	int fd = open(device, 0);
	char *buf;
	buf = (char *) malloc(400);
	int ret = ioctl(fd, IOCTL_LIST_ALL, buf);
	printf("%s\n", buf);
	//unsigned long num = 0;
	//buf->new_vector_id = 1;
	//ret = ioctl(fd, IOCTL_CHANGE_VECTOR, num);
	printf("%d\n", ret);
	return 0;
}

