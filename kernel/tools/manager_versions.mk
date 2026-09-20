# Static KernelSU version numbers.
#
# These are fixed constants and are NOT derived from the repository commit
# count anymore, so no recalibration is needed as the repository grows.
# The manager-specific versions are selected at runtime by signature index
# (see manager/manager_version.c for the authoritative per-manager table).
#
# NOTE: the values printed below are informational only. If you change a
# version/uapi_version in manager_version.c, update these lines to match.

# Base driver version, also reported for SukiSU-Ultra (signature index 0).
KSU_VERSION := 40900

$(info -- Default version: $(KSU_VERSION))
$(info -- Unknown manager fallback: version $(KSU_VERSION), uapi 4 (KERNEL_SU_UAPI_VERSION))
$(info -- Manager Versions: [version / uapi])
$(info --   [0]   SukiSU-Ultra:   $(KSU_VERSION) / 2)
$(info --   [1]   ReSukiSU:       35158 / 4)
$(info --   [2]   weishu/KSU:     32601 / 2)
$(info --   [3]   MKSU:           32604 / 2)
$(info --   [4]   xxKernelSU:     32640 / 4)
$(info --   [5]   RKSU:           32490 / 4)
$(info --   [6]   KowSU:          32673 / 4)
$(info --   [7]   MamboSU:        32601 / 4)
$(info --   [8]   KernelSU-Next:  33214 / 2)
$(info --   [9]   WildSU:         33208 / 2)
$(info --   [253] Dynamic 1:      $(KSU_VERSION) / 2)
$(info --   [254] Dynamic 2:      $(KSU_VERSION) / 2)
$(info --   [255] Dynamic 3:      $(KSU_VERSION) / 2)
