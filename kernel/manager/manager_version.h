#ifndef __KSU_MANAGER_VERSION_H
#define __KSU_MANAGER_VERSION_H

#include <linux/types.h>

#include "ksu.h"
#include "uapi/supercall.h"

// Get version number for specific manager by signature_index
int ksu_get_version_for_manager(u8 signature_index);

// Get UAPI version for specific manager by signature_index
u32 ksu_get_uapi_version_for_manager(u8 signature_index);

// Get default version (KERNEL_SU_VERSION)
static inline int ksu_get_default_version(void)
{
	return KERNEL_SU_VERSION;
}

// Get default UAPI version (KERNEL_SU_UAPI_VERSION)
static inline u32 ksu_get_default_uapi_version(void)
{
	return KERNEL_SU_UAPI_VERSION;
}

#endif
