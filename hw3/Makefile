VECTORS=VECTOR1 VECTOR2 VECTOR3 
all: HOOK_SYSCALL_TABLE IOCTLS CLONE2 $(VECTORS)

HOOK_SYSCALL_TABLE:
	cd hook_syscall_table && $(MAKE)
IOCTLS:
	cd ioctls && $(MAKE)
VECTOR1:
	cd vector1 && $(MAKE)
VECTOR2:
	cd vector2 && $(MAKE)
VECTOR3:
	cd vector3 && $(MAKE)
CLONE2:
	cd clone2 && $(MAKE)
clean:
	cd hook_syscall_table && $(MAKE) clean
	cd ioctls && $(MAKE) clean
	cd vector1 && $(MAKE) clean
	cd vector2 && $(MAKE) clean
	cd vector3 && $(MAKE) clean
	cd clone2 && $(MAKE) clean
