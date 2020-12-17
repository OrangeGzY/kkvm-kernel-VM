#include "../../err_signal.h"
#include<stdint.h>
#include "./do_syscall.h"

typedef asmlinkage long (*sys_call_ptr_t)(uint64_t arg1,uint64_t arg2,uint64_t arg3,uint64_t arg4,uint64_t arg5,uint64_t arg6);    /*ensure we use stack to pass arg*/

asmlinkage long sys_ni_syscall(void){return ERR_NO_SYSCALL;}

#define __SYSCALL(nr, sym) [SYS_##sym] =  sys_##sym 



static sys_call_ptr_t syscall_table[SYS_CALL_MAX_NUM] = {
    [0 ... SYS_CALL_MAX_NUM] = &sys_ni_syscall,
    __SYSCALL(1,exit),
};

void *_search_in_syscall_table(uint64_t nr){
        return syscall_table[nr];
};
