#ifndef __KSU_MANAGER_VERSION_H
#define __KSU_MANAGER_VERSION_H

#include <linux/types.h>

// Get version number for specific manager by signature_index
int ksu_get_version_for_manager(u8 signature_index);

// Get default version (KERNEL_SU_VERSION)
static inline int ksu_get_default_version(void)
{
	return KERNEL_SU_VERSION;
}

#endif
