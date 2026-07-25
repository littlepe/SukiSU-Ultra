#include <linux/types.h>

#include "supercall/internal.h"
#include "manager/manager_identity.h"
#include "policy/allowlist.h"

#include "compat/kernel_compat.h"

bool only_manager(void)
{
    bool result = is_manager();
    u32 uid = ksu_get_uid_t(current_uid());
    
    if (!result) {
        pr_warn("only_manager: DENIED - uid=%u is_manager=%d\n", uid, result);
    } else {
        pr_info("only_manager: ALLOWED - uid=%u\n", uid);
    }
    
    return result;
}

bool only_root(void)
{
    return ksu_get_uid_t(current_uid()) == 0;
}

bool manager_or_root(void)
{
    return ksu_get_uid_t(current_uid()) == 0 || is_manager();
}

bool always_allow(void)
{
    return true;
}

bool allowed_for_su(void)
{
    bool is_allowed = is_manager() || ksu_is_allow_uid_for_current(ksu_get_uid_t(current_uid()));

    return is_allowed;
}
