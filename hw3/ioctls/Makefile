obj-m += ioctls.o
KBUILD_EXTRA_SYMBOLS := /usr/src/hw3-cse506p35/hw3/hook_syscall_table/Module.symvers
all:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
