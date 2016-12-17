#ifndef ___VECTOR_SYS_TABLE_HEADER__
#define __VECTOR_SYS_TABLE_HEADER__

#include "../sys_ops.h"

struct vector_systable {
	int id;
	int ref_cnt;
	struct sys_ops ops;
	struct module *vector_module;
	struct list_head vector_list;
};

#endif
