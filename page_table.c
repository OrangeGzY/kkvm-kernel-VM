#include <err.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "debug.h"

#define _BITSET(x)	((1) << (x))
#define CR4_PAE_BIT	 _BITSET(5)

__always_inline void set_control_registers(struct kvm_sregs *p_sregs){
    p_sregs->cr4 = p_sregs->cr4 | CR4_PAE_BIT;  /*  设置CR4的第五位，打开PAE    */
    #ifdef DEBUG_0
    printf("sregs->cr4:0x%llx\n",p_sregs->cr4);   
    #endif
}
int initialize_page_table(void *mem,struct kvm_sregs *p_sregs){
    set_control_registers(p_sregs);
}