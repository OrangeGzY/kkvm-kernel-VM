#include <linux/kvm.h>
#include <stdint.h>
typedef struct VM{
    void *mem;
    uint64_t mem_size;
    int vcpufd;
    int vmfd;
    struct kvm_run *run;
}VM;
