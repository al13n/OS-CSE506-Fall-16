#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../ioctls/ioctls.h"
#include "../ioctls_s.h"

int fd;

int main(int argc, char **argv)
{
       // signal(SIGINT, _sigint_handler);
	char device[] = "/dev/ioctls";
        fd = open(device, 0);
	int ret = ioctl(fd, IOCTL_CHANGE_VECTOR, 1);
	mkdir("v1directory", 0700);
	rmdir("v1directory");
	symlink("hello.protected", "justafile");
	unlink("deletefile");
	FILE *fp;
	fp = fopen("justafile", "r");
	fclose(fp);
	char *buf1 = (char *)malloc(5000);
	ret = ioctl(fd, IOCTL_LIST_ALL, buf1);
	printf("%s\n", buf1);
	struct ioctls_s buf;
	buf.buf = (char *)malloc(4096);
	buf.pid = getpid();
        ret = ioctl(fd, IOCTL_PROC_VECTOR, &buf);
	printf("%s\n", buf.buf);
	//while(1) sleep(1);
	ret = ioctl(fd, IOCTL_CHANGE_VECTOR, 0);
	return 0;
}
