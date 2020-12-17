#include "./do_syscall.h"
#include "../../err_signal.h"
#include <stdint.h>
#include "syscall_table.c"

int go_to_syscall_table(uint64_t sys_num){
    void * syscall_jump = syscall_jump = _search_in_syscall_table(sys_num);
    int ret_val;
    ret_val = (
       ((uint64_t (*)
        (uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t))
        syscall_jump ) 
        (arg1, arg2,arg3, arg4,arg5,arg6)
    
    )
    return ret_val;
}


int do_syscall(
    
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6
){
    uint64_t syscall_num;
    
    asm(
    "mov %[syscall_num], rax;"
    : [syscall_num] "=r" (syscall_num)  /*output operand*/
    );

    
    if(syscall_num > SYS_CALL_MAX_NUM || syscall_num < 0){return ERR_SYS_EXCEED;}
    
    /* attention! syscall num is a index in syscall_table */
    return go_to_syscall_table(syscall_num);

}
