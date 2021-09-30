#include "kernel/calls.h"
#include "kernel/task.h"
#include "kernel/aio.h"
#include "fs/aio.h"

dword_t sys_io_setup(dword_t nr_events, addr_t ctx_idp) {
    struct aioctx *ctx = aioctx_new(nr_events);
    if (ctx == NULL) return _ENOMEM;
    if (IS_ERR(ctx)) return PTR_ERR(ctx);

    int ctx_id = aioctx_table_insert(current->aioctx, ctx);
    if (ctx_id < 0) {
        aioctx_release(ctx);
        return ctx_id;
    }

    dword_t ctx_id_guest = (dword_t)ctx_id;
    if (ctx_idp && user_write(ctx_idp, (char*)&ctx_id_guest, sizeof(dword_t)))
        return _EFAULT;

    return 0;
}

dword_t sys_io_destroy(addr_t p_ctx_id) {
    unsigned int ctx_id = 0;
    if (user_read(p_ctx_id, &ctx_id, sizeof(ctx_id))) return _EFAULT;
    
    int err = aioctx_table_remove(current->aioctx, ctx_id) < 0;
    if (err < 0) {
        return err;
    }

    return 0;
}

dword_t sys_io_getevents(addr_t ctx_id, dword_t min_nr, dword_t nr, addr_t events, addr_t timeout) {
    return _ENOSYS;
}

dword_t sys_io_submit(addr_t ctx_id, dword_t nr, addr_t iocbpp) {
    return _ENOSYS;
}

dword_t sys_io_cancel(addr_t ctx_id, addr_t iocb, addr_t result) {
    return _ENOSYS;
}