ifndef GIT_BIN
GIT_BIN := /usr/bin/env PATH="$$PATH":/usr/bin:/usr/local/bin git
endif

ifndef KSU_LOCAL_VERSION
$(error KSU_LOCAL_VERSION must be defined before including this file)
endif

# Formula: version = base + KSU_LOCAL_VERSION + offset
SUKISU_ULTRA_OFFSET := -3096
RESUKISU_OFFSET := 438
WEISHU_KSU_OFFSET := -1331
MKSU_OFFSET := -1344
XXKSU_OFFSET := -1300
RKSU_OFFSET := -1264
KOWSU_OFFSET := -1296
NEXT_OFFSET := -669
WILDSU_OFFSET := -776

# Calculate default version
KSU_VERSION := $(shell expr 40000 + $(KSU_LOCAL_VERSION) + $(SUKISU_ULTRA_OFFSET))

# Calculate preview versions for display
KSU_VERSION_SHIRKNEKO := $(KSU_VERSION)
KSU_VERSION_RESUKISU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(RESUKISU_OFFSET))
KSU_VERSION_WEISHU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(WEISHU_KSU_OFFSET))
KSU_VERSION_MKSU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(MKSU_OFFSET))
KSU_VERSION_XXKSU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(XXKSU_OFFSET))
KSU_VERSION_RKSU := $(shell expr 30100 + $(KSU_LOCAL_VERSION) + $(RKSU_OFFSET))
KSU_VERSION_KOWSU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(KOWSU_OFFSET))
KSU_VERSION_MAMBOSU := $(shell expr 40000 + $(KSU_LOCAL_VERSION))
KSU_VERSION_NEXT := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(NEXT_OFFSET))
KSU_VERSION_WILDSU := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + $(WILDSU_OFFSET))

# Display version information
$(info -- Current repo commits: $(KSU_LOCAL_VERSION))
$(info -- Default version: $(KSU_VERSION))
$(info -- Manager Versions:)
$(info --   [0] SukiSU-Ultra:    $(KSU_VERSION_SHIRKNEKO) = 40000+$(KSU_LOCAL_VERSION)$(SUKISU_ULTRA_OFFSET))
$(info --   [1] ReSukiSU:        $(KSU_VERSION_RESUKISU) = 30000+$(KSU_LOCAL_VERSION)+$(RESUKISU_OFFSET))
$(info --   [2] weishu/KSU:      $(KSU_VERSION_WEISHU) = 30000+$(KSU_LOCAL_VERSION)$(WEISHU_KSU_OFFSET))
$(info --   [3] MKSU:            $(KSU_VERSION_MKSU) = 30000+$(KSU_LOCAL_VERSION)$(MKSU_OFFSET))
$(info --   [4] xxKernelSU:      $(KSU_VERSION_XXKSU) = 30000+$(KSU_LOCAL_VERSION)$(XXKSU_OFFSET))
$(info --   [5] RKSU:            $(KSU_VERSION_RKSU) = 30100+$(KSU_LOCAL_VERSION)$(RKSU_OFFSET))
$(info --   [6] KowSU:           $(KSU_VERSION_KOWSU) = 30000+$(KSU_LOCAL_VERSION)$(KOWSU_OFFSET))
$(info --   [7] MamboSU:         $(KSU_VERSION_MAMBOSU) = 40000+$(KSU_LOCAL_VERSION))
$(info --   [8] KernelSU-Next:   $(KSU_VERSION_NEXT) = 30000+$(KSU_LOCAL_VERSION)$(NEXT_OFFSET))
$(info --   [9] WildSU:          $(KSU_VERSION_WILDSU) = 30000+$(KSU_LOCAL_VERSION)$(WILDSU_OFFSET))
