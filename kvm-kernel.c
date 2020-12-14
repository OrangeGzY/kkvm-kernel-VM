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

//#define DEBUG 
void kvm_check(int kvm){
    int ret;
    /* 检查kvm版本 */
    ret = ioctl(kvm, KVM_GET_API_VERSION, NULL);
    if (ret == -1)
	    err(1, "KVM_GET_API_VERSION");
    if (ret != 12)
	    errx(1, "KVM_GET_API_VERSION %d, expected 12", ret);

    /* 检查kvm扩展 */
    ret = ioctl(kvm, KVM_CHECK_EXTENSION, KVM_CAP_USER_MEMORY);
    if (ret == -1)
	    err(1, "KVM_CHECK_EXTENSION");
    if (!ret)
	    errx(1, "Required extension KVM_CAP_USER_MEM not available");
}
void kvm_kernel(uint8_t code[]){

    int kvm, vmfd, vcpufd, ret;
    uint8_t *mem;
    struct kvm_sregs sregs;
    size_t mmap_size;
    struct kvm_run *run;
    
    
    kvm = open("/dev/kvm",O_RDWR|O_CLOEXEC); //打开一个kvm
    printf("kvmfd:%d\n",kvm);
    kvm_check(kvm);
    /* 创建一个VM，用来代表所有与仿真系统相关的内容，包括内存和cpu */
    /* VM通过文件描述符的形式向我们返回一个VM的句柄 */
    vmfd = ioctl(kvm, KVM_CREATE_VM, (unsigned long)0);

    /* 分配一个页 */
    mem = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("Give KVM:%p\n",mem);

    /* 将code拷贝过去 */
    memcpy(mem,code,12);

    /*  通知VM他的新内存 */
    struct kvm_userspace_memory_region region = {
	.slot = 0,
	.guest_phys_addr = 0x1000,  /* 告诉vm从guest上看到的物理内存地址是0x1000(第二页) */
	.memory_size = 0x1000,
	.userspace_addr = (uint64_t)mem,
    };
    //printf("VM userspace addr:0x%lx\n",(uint64_t)mem);
    ioctl(vmfd, KVM_SET_USER_MEMORY_REGION, &region);

    /*  添加一个vCPU  */
    /*  每个vCPU关联一个kvm_vcpu，其中有一个很重要的成员struct kvm_run  */
    /*  当vmexit时，kvm_run会包含一些很重要的信息比如vmexit的原因等，所以我们将他map到userspace */
    /*  https://elixir.bootlin.com/linux/latest/source/include/uapi/linux/kvm.h#L263    */
    vcpufd = ioctl(vmfd, KVM_CREATE_VCPU, (unsigned long)0);

    /* 获取需要map的大小 */
    mmap_size = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, NULL);
    
    /* run是一个指向kvm_run的指针，此时将vcpu对应的 kvm_run映射到用户空间 */
    run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpufd, 0);

    /*  在开始之前，我们还需要对寄存器进行设置  */
    /*  这里与x86 linux不同，x86linux需要复位后复位后的cs:ip此时逻辑地址为：0xffff0000:0xfff0即4GB-16字节，即第一条代码：reset vector   */
    ioctl(vcpufd, KVM_GET_SREGS, &sregs);   /*  kvm_arch_vcpu_ioctl_get_sregs(vcpu, kvm_sregs)  返回special regs    */
    sregs.cs.base=0;                        /*  cs指向0地址  */
    sregs.cs.selector=0;
    ioctl(vcpufd, KVM_SET_SREGS, &sregs);   /*  同步我们的设置  */

    struct kvm_regs regs = {
	.rip = 0x1000,
	.rax = 2,
	.rbx = 2,
	.rflags = 0x2,
    };                                      /*   rax、rbx、rflags为x86要求，这里不设置成这样vm就起不来 */
    ioctl(vcpufd, KVM_SET_REGS, &regs);     /*  设置其他寄存器的值，rip指向我们start code的地址0x1000（此时cs=0）   */
    
    #ifdef DEBUG
    printf("pid:%d\n",getpid());
    #endif     

    while(1){
        	ioctl(vcpufd, KVM_RUN, NULL);
            /*  run is a kvm_run *  */
	        switch (run->exit_reason) {
	        
                case KVM_EXIT_IO:
                    /*  用这里来模拟串口的通信  */
                    if(run->io.direction == KVM_EXIT_IO_OUT /*  out  */
                        && run->io.size == 1 
                        && run->io.port == 0x3f8            /*  串口编号   */
                        && run->io.count == 1){
           
                        #ifdef DEBUG
                        printf("run->io.data_offset:0x%lx\n",run->io.data_offset);
                        printf("run:%p\n",run);
                        printf("putchar addr: %p\n",run+run->io.data_offset);
                        #endif

                        putchar(
                                *(  ((char*)run) + run->io.data_offset)
                        );
                            
                    }else{
                        errx(1,"Cannot handle is KVM_EXIT_IO");
                    }
                    break;


                case KVM_EXIT_FAIL_ENTRY:
                        errx(1, "KVM_EXIT_FAIL_ENTRY: hardware_entry_failure_reason = 0x%llx",
                        (unsigned long long)run->fail_entry.hardware_entry_failure_reason);

                case KVM_EXIT_INTERNAL_ERROR:
	                    errx(1, "KVM_EXIT_INTERNAL_ERROR: suberror = 0x%x",
	                    run->internal.suberror);

                case KVM_EXIT_HLT:
	                puts("KVM_EXIT_HLT");
	                return 0;
	    }
    }


}
int main(){
    const uint8_t code[] = {
	0xba, 0xf8, 0x03, /* mov $0x3f8, %dx */
	0x00, 0xd8,       /* add %bl, %al */
	0x04, '0',        /* add $'0', %al */
	0xee,             /* out %al, (%dx) */
	0xb0, '\n',       /* mov $'\n', %al */
	0xee,             /* out %al, (%dx) */
	0xf4,             /* hlt */
    };
   
   kvm_kernel(code);
}
