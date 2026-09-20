#include <linux/version.h>
#include <linux/types.h>
#include "ksu.h"
#include "uapi/supercall.h"
#include "manager/manager_version.h"

/*
 * Static per-manager version numbers.
 *
 * These are fixed constants and are no longer derived from the repository
 * commit count. The correct values are still selected dynamically at runtime
 * according to the manager signature of the calling process.
 *
 * version:      value reported in ksu_get_info_cmd.version
 * uapi_version: value reported in ksu_get_info_cmd.uapi_version
 *               (4 is the current default, KERNEL_SU_UAPI_VERSION)
 *
 * To change a version, edit the value here.
 */
struct manager_version_entry {
	u8 signature_index;
	int version;
	u32 uapi_version;
};

static const struct manager_version_entry manager_version_table[] = {
	{ 0,   KSU_VERSION, 2 },  // SukiSU-Ultra (default/base version)
	{ 1,   35158,       4 },  // ReSukiSU
	{ 2,   32601,       2 },  // weishu/KSU (Official)
	{ 3,   32604,       2 },  // MKSU
	{ 4,   32640,       4 },  // xxKernelSU
	{ 5,   32490,       4 },  // RKSU
	{ 6,   32673,       4 },  // KowSU
	{ 7,   32601,       4 },  // MamboSU
	{ 8,   33214,       2 },  // KernelSU-Next
	{ 9,   33208,       2 },  // WildSU
	{ 253, KSU_VERSION, 2 },  // Dynamic Manager 1
	{ 254, KSU_VERSION, 2 },  // Dynamic Manager 2
	{ 255, KSU_VERSION, 2 },  // Dynamic Manager 3
};

// Get version number for specific manager by signature_index
int ksu_get_version_for_manager(u8 signature_index)
{
	int i;

	/* Search for signature_index in table */
	for (i = 0; i < ARRAY_SIZE(manager_version_table); i++) {
		if (manager_version_table[i].signature_index == signature_index) {
			return manager_version_table[i].version;
		}
	}

	/* Unknown managers use default version */
	return KERNEL_SU_VERSION;
}

// Get UAPI version for specific manager by signature_index
u32 ksu_get_uapi_version_for_manager(u8 signature_index)
{
	int i;

	/* Search for signature_index in table */
	for (i = 0; i < ARRAY_SIZE(manager_version_table); i++) {
		if (manager_version_table[i].signature_index == signature_index) {
			return manager_version_table[i].uapi_version;
		}
	}

	/* Unknown managers use default UAPI version */
	return KERNEL_SU_UAPI_VERSION;
}
