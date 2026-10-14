#include <linux/slab.h>
#include <linux/rculist.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include "manager_identity.h"
#include "ksu.h"
#include "uapi/supercall.h"
#include "compat/kernel_compat.h"

u16 ksu_last_manager_appid = KSU_INVALID_APPID;

struct ksu_manager_node {
    u8 signature_index;
    u16 appid;
    struct list_head list;
    struct rcu_head rcu;
};

static LIST_HEAD(ksu_manager_appid_list);
static DEFINE_SPINLOCK(ksu_manager_list_write_lock);

bool ksu_is_manager_appid(u16 appid)
{
    bool found = false;
    struct ksu_manager_node *pos;

    rcu_read_lock();
    list_for_each_entry_rcu (pos, &ksu_manager_appid_list, list) {
        if (pos->appid == appid) {
            found = true;
            break;
        }
    }
    rcu_read_unlock();

    return found;
}

bool ksu_is_manager_uid(u32 uid)
{
    u16 appid = uid % PER_USER_RANGE;

    return ksu_is_manager_appid(appid);
}

bool is_manager(void)
{
    return ksu_is_manager_uid(ksu_get_uid_t(current_uid()));
}

void ksu_register_manager(u32 uid, u8 signature_index)
{
    struct ksu_manager_node *node;
    u16 appid;

    if (ksu_is_manager_uid(uid)) {
        pr_info("register_manager: uid=%u already registered\n", uid);
        return;
    }

    node = kzalloc(sizeof(*node), GFP_ATOMIC);
    if (unlikely(!node)) {
        pr_err("register_manager: kzalloc failed for uid=%u\n", uid);
        return;
    }

    appid = uid % PER_USER_RANGE;

    node->appid = appid;
    node->signature_index = signature_index;

    spin_lock(&ksu_manager_list_write_lock);

    if (ksu_is_manager_uid(uid)) {
        spin_unlock(&ksu_manager_list_write_lock);
        kfree(node);
        pr_info("register_manager: uid=%u already registered (race)\n", uid);
        return;
    }

    list_add_tail_rcu(&node->list, &ksu_manager_appid_list);

    spin_unlock(&ksu_manager_list_write_lock);

    if (ksu_last_manager_appid == KSU_INVALID_APPID) {
        ksu_last_manager_appid = appid;
        pr_info("last_manager_appid: set to %u (by register uid=%u)\n", (u32)ksu_last_manager_appid, uid);
    } else if (ksu_last_manager_appid != appid) {
        pr_info("last_manager_appid: kept %u, new manager uid=%u appid=%u index=%u not preferred\n",
                (u32)ksu_last_manager_appid, uid, (unsigned)appid, (unsigned)signature_index);
    }
    
    pr_info("register_manager: SUCCESS - uid=%u appid=%u signature_index=%u\n", 
            uid, appid, signature_index);
    return;
}

void ksu_unregister_manager(u32 uid)
{
    struct ksu_manager_node *node, *pos, *tmp;
    bool mark_another_manager = false;

    if (!ksu_is_manager_uid(uid))
        return;

    u16 appid = uid % PER_USER_RANGE;

    pr_info("unregister_manager: uid=%u appid=%u, last_manager_appid=%u\n", uid, (unsigned)appid,
            (u32)ksu_last_manager_appid);

    if (ksu_last_manager_appid == appid)
        mark_another_manager = true;

    spin_lock(&ksu_manager_list_write_lock);

    list_for_each_entry_safe (pos, tmp, &ksu_manager_appid_list, list) {
        if (pos->appid == appid) {
            list_del_rcu(&pos->list);
            spin_unlock(&ksu_manager_list_write_lock);
            kfree_rcu(pos, rcu);
            pr_info("unregister_manager: removed appid=%u, last_manager_appid still %u\n", (unsigned)appid,
                    (u32)ksu_last_manager_appid);
            return;
        }

        if (mark_another_manager) {
            /* pick another still-registered manager, not the one being removed */
            ksu_last_manager_appid = pos->appid;
            mark_another_manager = false;
        }
    }

    spin_unlock(&ksu_manager_list_write_lock);

    if (mark_another_manager)
        ksu_last_manager_appid = KSU_INVALID_APPID;
    return;
}

void ksu_unregister_manager_by_signature_index(u8 signature_index)
{
    struct ksu_manager_node *node, *pos, *tmp;
    bool mark_another_manager = false;
    u16 last_each_alive_appid = KSU_INVALID_APPID;

    spin_lock(&ksu_manager_list_write_lock);

    list_for_each_entry_safe (pos, tmp, &ksu_manager_appid_list, list) {
        if (pos->signature_index == signature_index) {
            if (pos->appid == ksu_last_manager_appid) {
                mark_another_manager = true;
            }

            list_del_rcu(&pos->list);
            kfree_rcu(pos, rcu);
        } else {
            last_each_alive_appid = pos->appid;
        }
    }

    spin_unlock(&ksu_manager_list_write_lock);

    if (mark_another_manager)
        ksu_last_manager_appid = last_each_alive_appid;
}

void ksu_unregister_all_manager(void)
{
    struct ksu_manager_node *pos, *tmp;

    pr_info("unregister_all_manager: clearing list, last_manager_appid=%u -> reset to INVALID\n",
            (u32)ksu_last_manager_appid);

    spin_lock(&ksu_manager_list_write_lock);

    list_for_each_entry_safe (pos, tmp, &ksu_manager_appid_list, list) {
        list_del_rcu(&pos->list);
        kfree_rcu(pos, rcu);
    }

    spin_unlock(&ksu_manager_list_write_lock);

    /*
     * Every entry is gone, so a stale appid here would never resolve again
     * (register_manager only assigns while the value is INVALID).
     */
    ksu_last_manager_appid = KSU_INVALID_APPID;
}

bool ksu_has_manager(void)
{
    bool empty;

    rcu_read_lock();
    empty = list_empty(&ksu_manager_appid_list);
    rcu_read_unlock();

    return !empty;
}

int ksu_handle_get_managers_cmd(struct ksu_get_managers_cmd __user *arg, struct ksu_get_managers_cmd *cmd)
{
    struct ksu_manager_node *pos;
    struct ksu_manager_entry *entries;
    u16 max_allowed = cmd->count;
    int count = 0;
    int copy_count;

    entries = kcalloc(max_allowed, sizeof(*entries), GFP_KERNEL);
    if (!entries && max_allowed)
        return -ENOMEM;

    rcu_read_lock();
    list_for_each_entry_rcu (pos, &ksu_manager_appid_list, list) {
        if (count < max_allowed) {
            entries[count].uid = pos->appid;
            entries[count].signature_index = pos->signature_index;
        }

        count++;
    }
    rcu_read_unlock();

    cmd->total_count = count;

    copy_count = min_t(int, count, max_allowed);

    if (copy_count && copy_to_user((char __user *)arg + sizeof(*cmd), entries, copy_count * sizeof(*entries))) {
        kfree(entries);
        return -EFAULT;
    }

    kfree(entries);
    return 0;
}

int ksu_get_manager_signature_index_by_appid(u16 appid)
{
    struct ksu_manager_node *pos;

    rcu_read_lock();
    list_for_each_entry_rcu (pos, &ksu_manager_appid_list, list) {
        if (pos->appid == appid) {
            rcu_read_unlock();
            return pos->signature_index;
        }
    }
    rcu_read_unlock();
    return -ENODATA;
}

int ksu_get_preferred_manager_signature_index(void)
{
    struct ksu_manager_node *pos;
    int index = -ENODATA;

    if (ksu_last_manager_appid != KSU_INVALID_APPID) {
        index = ksu_get_manager_signature_index_by_appid(ksu_last_manager_appid);
        pr_info("preferred_manager: last_manager_appid=%u -> index=%d\n", (u32)ksu_last_manager_appid, index);
        if (index >= 0)
            return index;
    } else {
        pr_info("preferred_manager: last_manager_appid is INVALID\n");
    }

    /*
     * last_manager_appid can be INVALID while the list is already populated
     * (register assigns it outside the lock, or after unregister_all reset).
     * Never fall through to a bogus default: use any registered manager.
     */
    rcu_read_lock();
    list_for_each_entry_rcu (pos, &ksu_manager_appid_list, list) {
        index = pos->signature_index;
        pr_info("preferred_manager: fallback to list entry appid=%u index=%d\n", (unsigned)pos->appid, index);
        break;
    }
    rcu_read_unlock();

    return index;
}
