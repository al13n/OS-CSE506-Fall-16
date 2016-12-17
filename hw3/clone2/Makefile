obj-m += sys_clone2.o
KBUILD_EXTRA_SYMBOLS := /usr/src/hw3-cse506p35/hw3/hook_syscall_table/Module.symvers
INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: clone2_user clone2

clone2_user: clone2_user.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi clone2_user.c -o clone2_user.o

clone2:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f clone2_user.o

