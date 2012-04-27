#ifndef __ARM_INIT_H__
#define __ARM_INIT_H__

#include <linux/init.h>

#define __define_machine_initcall(mach,level,fn,id) \
	static int __init __machine_initcall_##mach##_##fn(void) {	\
		if (of_have_populated_dt())				\
			return 0;					\
		if (machine_is_##mach())				\
			return fn();					\
		return 0;						\
	}								\
	__define_initcall(level,__machine_initcall_##mach##_##fn,id);

#define machine_core_initcall(mach,fn)		__define_machine_initcall(mach,"1",fn,1)
#define machine_core_initcall_sync(mach,fn)	__define_machine_initcall(mach,"1s",fn,1s)
#define machine_postcore_initcall(mach,fn)	__define_machine_initcall(mach,"2",fn,2)
#define machine_postcore_initcall_sync(mach,fn)	__define_machine_initcall(mach,"2s",fn,2s)
#define machine_arch_initcall(mach,fn)		__define_machine_initcall(mach,"3",fn,3)
#define machine_arch_initcall_sync(mach,fn)	__define_machine_initcall(mach,"3s",fn,3s)
#define machine_subsys_initcall(mach,fn)	__define_machine_initcall(mach,"4",fn,4)
#define machine_subsys_initcall_sync(mach,fn)	__define_machine_initcall(mach,"4s",fn,4s)
#define machine_fs_initcall(mach,fn)		__define_machine_initcall(mach,"5",fn,5)
#define machine_fs_initcall_sync(mach,fn)	__define_machine_initcall(mach,"5s",fn,5s)
#define machine_rootfs_initcall(mach,fn)	__define_machine_initcall(mach,"rootfs",fn,rootfs)
#define machine_device_initcall(mach,fn)	__define_machine_initcall(mach,"6",fn,6)
#define machine_device_initcall_sync(mach,fn)	__define_machine_initcall(mach,"6s",fn,6s)
#define machine_late_initcall(mach,fn)		__define_machine_initcall(mach,"7",fn,7)
#define machine_late_initcall_sync(mach,fn)	__define_machine_initcall(mach,"7s",fn,7s)

#define __define_compatible_initcall(compatible,level,fn,id) \
	static int __init __compatible_initcall_##mach##_##fn(void) {	\
		struct device_node *np =				\
			of_find_matching_node(NULL, compatible);	\
		if (!np)						\
			return 0;					\
		return fn();						\
	}								\
	__define_initcall(level,__compatible_initcall_##mach##_##fn,id);

#define compatible_core_initcall(mach,fn)		__define_compatible_initcall(mach,"1",fn,1)
#define compatible_core_initcall_sync(mach,fn)		__define_compatible_initcall(mach,"1s",fn,1s)
#define compatible_postcore_initcall(mach,fn)		__define_compatible_initcall(mach,"2",fn,2)
#define compatible_postcore_initcall_sync(mach,fn)	__define_compatible_initcall(mach,"2s",fn,2s)
#define compatible_arch_initcall(mach,fn)		__define_compatible_initcall(mach,"3",fn,3)
#define compatible_arch_initcall_sync(mach,fn)		__define_compatible_initcall(mach,"3s",fn,3s)
#define compatible_subsys_initcall(mach,fn)		__define_compatible_initcall(mach,"4",fn,4)
#define compatible_subsys_initcall_sync(mach,fn)	__define_compatible_initcall(mach,"4s",fn,4s)
#define compatible_fs_initcall(mach,fn)			__define_compatible_initcall(mach,"5",fn,5)
#define compatible_fs_initcall_sync(mach,fn)		__define_compatible_initcall(mach,"5s",fn,5s)
#define compatible_rootfs_initcall(mach,fn)		__define_compatible_initcall(mach,"rootfs",fn,rootfs)
#define compatible_device_initcall(mach,fn)		__define_compatible_initcall(mach,"6",fn,6)
#define compatible_device_initcall_sync(mach,fn)	__define_compatible_initcall(mach,"6s",fn,6s)
#define compatible_late_initcall(mach,fn)		__define_compatible_initcall(mach,"7",fn,7)
#define compatible_late_initcall_sync(mach,fn)		__define_compatible_initcall(mach,"7s",fn,7s)

#endif /* __ARM_INIT_H__ */
