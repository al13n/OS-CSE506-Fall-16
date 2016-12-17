#ifndef __IOCTLS_H__
#define __IOCTLS_H_

#define DEVICE_NAME "ioctls"
#define DEVICE_NUM 9
#define IOCTL_CHANGE_VECTOR _IOR(DEVICE_NUM, 1, char*)
#define IOCTL_LIST_ALL _IOR(DEVICE_NUM, 2, char*)
#define IOCTL_PROC_VECTOR _IOR(DEVICE_NUM, 3, char*)

#ifdef __IOCTLS_C__
extern int list_all_vectors(char *);
extern int reduce_ref_count(int);
extern int increment_ref_count(int);
#endif

#endif
