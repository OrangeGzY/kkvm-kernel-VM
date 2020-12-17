#include "syscall_init.c"
#include <stdint.h>
#define SYSCALL_SUCCCESS 0





int kernel_enter(uint64_t argc,char *argv[]){
    int ret;
//    enable_paging();

    ret = syscall_init();
    if(!ret){
    printf("success to init syscallllllll!\n");    
}else{return -1;}    



}
