#include <linux/version.h>
#include <linux/types.h>
#include "ksu.h"
#include "klog.h" // IWYU pragma: keep
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
	{ 0,   KSU_VERSION, 5 },  // SukiSU-Ultra https://github.com/SukiSU-Ultra/SukiSU-Ultra (default/base version)
	{ 1,   35229,       5 },  // BakaSU https://github.com/Baka-SU/BakaSU
	{ 2,   32680,       5 },  // weishu/KSU https://github.com/tiann/KernelSU (Official)
	{ 3,   32604,       2 },  // MKSU https://github.com/5ec1cff/KernelSU
	{ 4,   32680,       5 },  // xxKernelSU https://github.com/backslashxx/KernelSU
	{ 5,   32490,       4 },  // RKSU https://github.com/rsuntk/KernelSU
	{ 6,   32715,       5 },  // KowSU https://github.com/KowX712/KernelSU
	{ 7,   32543,       4 },  // MamboSU https://github.com/rapliVx/KernelSU
	{ 8,   33355,       5 },  // KernelSU-Next https://github.com/KernelSU-Next/KernelSU-Next
	{ 9,   33208,       2 },  // WildSU https://github.com/KernelSU-Next/KernelSU-Next
	{ 253, KSU_VERSION, 5 },  // Dynamic Manager 1
	{ 254, KSU_VERSION, 5 },  // Dynamic Manager 2
	{ 255, KSU_VERSION, 5 },  // Dynamic Manager 3
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
	pr_info("manager_version: signature_index=%u not in table, default uapi=%u\n",
		(unsigned)signature_index, KERNEL_SU_UAPI_VERSION);
	return KERNEL_SU_UAPI_VERSION;
}
