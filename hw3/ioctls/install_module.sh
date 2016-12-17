#!bin/bash
rm /dev/ioctls
rmmod ioctls
insmod ioctls/ioctls.ko
mknod /dev/ioctls c 9 12
