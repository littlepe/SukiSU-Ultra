#include <linux/version.h>
#include <linux/types.h>
#include "ksu.h"
#include "manager/manager_version.h"

// Current repository commit count
#ifndef KSU_LOCAL_COMMITS
#define KSU_LOCAL_COMMITS 0
#error "KSU_LOCAL_COMMITS not defined! Check manager/Kbuild"
#endif

// Compile-time verification
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#pragma message "KSU_LOCAL_COMMITS = " TOSTRING(KSU_LOCAL_COMMITS)

/*
 * Formula: version = base + KSU_LOCAL_COMMITS + offset
 * To add a new manager: add an entry with signature_index, base, and offset
 */
struct manager_version_entry {
	u8 signature_index;
	int base;
	int offset;
};

static const struct manager_version_entry manager_version_table[] = {
	{ 0,   40000, -3094 },  // SukiSU-Ultra
	{ 1,   30000,  1148 },  // ReSukiSU
	{ 2,   30000, -1365 },  // weishu/KSU (Official)
	{ 3,   30000, -1342 },  // MKSU
	{ 4,   30000, -1298 },  // xxKernelSU
	{ 5,   30100, -1262 },  // RKSU
	{ 6,   30000, -1294 },  // KowSU
	{ 7,   40000,     0 },  // MamboSU
	{ 8,   30000,  -667 },  // KernelSU-Next
	{ 9,   30000,  -774 },  // WildSU
	{ 253, 40000,     0 },  // Dynamic Manager 1
	{ 254, 40000,     0 },  // Dynamic Manager 2
	{ 255, 40000,     0 },  // Dynamic Manager 3
};

// Get version number for specific manager by signature_index
int ksu_get_version_for_manager(u8 signature_index)
{
	int i;
	
	/* Search for signature_index in table */
	for (i = 0; i < ARRAY_SIZE(manager_version_table); i++) {
		if (manager_version_table[i].signature_index == signature_index) {
			/* Version = base + KSU_LOCAL_COMMITS + offset */
			return manager_version_table[i].base + 
			       KSU_LOCAL_COMMITS + 
			       manager_version_table[i].offset;
		}
	}
	
	/* Unknown managers use default version */
	return KERNEL_SU_VERSION;
}
