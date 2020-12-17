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
#include "./page_table.c"
#include "./vm.h"
#define MEM_SIZE 0x40000000    // 400M
                 
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
void kvm_kernel(uint8_t code[],size_t len){

    int kvm, vmfd, vcpufd, ret;
    uint8_t *mem;
    struct kvm_sregs sregs;
   // struct kvm_regs regs;
    size_t mmap_size;
    struct kvm_run *run;
    
    
    
    kvm = open("/dev/kvm",O_RDWR|O_CLOEXEC); //打开一个kvm

    //printf("kvmfd:%d\n",kvm);
    kvm_check(kvm);
    /* 创建一个VM，用来代表所有与仿真系统相关的内容，包括内存和cpu */
    /* VM通过文件描述符的形式向我们返回一个VM的句柄 */
    vmfd = ioctl(kvm, KVM_CREATE_VM, (unsigned long)0);

    
    /* 分配4M匿名共享内存，下面会将这段共享内存映射到guest中，作为客户机看到的物理内存   */
   
    mem = mmap(NULL, MEM_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("Give KVM phy_addr as:%p\n",mem);

    /* 将code拷贝到host分给geust的地址（在guest角度看是GPA，0地址的位置） */
    //memcpy(mem,code,12);
    memcpy(mem,code,len);

    /*  通知VM他的新内存 */
    struct kvm_userspace_memory_region region = {
	.slot = 0,                  /*  对应slot=0，建立的是GPA到HVA的关系，通过kvm_mem_slot结构体保存，id_to_memslot()负责将用户态的slot转成一个kvm_mem_slot结构  */
	.guest_phys_addr = 0,       /* 告诉vm从guest上看到的物理内存地址是 0 地址 */
	.memory_size = MEM_SIZE,
	.userspace_addr = (uint64_t)mem,
    };
    //printf("VM userspace addr:0x%lx\n",(uint64_t)mem);
    /*  建立guest物理地址空间中的内存区域与qemu-kvm虚拟地址空间中的内存区域的映射   */
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
    struct kvm_regs regs;
    ioctl(vcpufd, KVM_GET_REGS, &regs);
    regs.rip = 0;
    regs.rsp = 0x200000; // stack address
    regs.rflags = 0x2; // in x86 the 0x2 bit should always be set
    ioctl(vcpufd, KVM_SET_REGS, &regs);     /*  设置其他寄存器的值，rip指向我们start code的地址0（此时cs=0）   */




    /*  这里与x86 linux不同，x86linux需要复位后复位后的cs:ip此时逻辑地址为：0xffff0000:0xfff0即4GB-16字节，即第一条代码：reset vector   */
    ioctl(vcpufd, KVM_GET_SREGS, &sregs);   /*  kvm_arch_vcpu_ioctl_get_sregs(vcpu, kvm_sregs)  返回special regs    */
    sregs.cs.base=sregs.cs.selector=0;                  /*  cs指向0地址  */
    initialize_page_table(mem,&sregs);      /*  初始化页表,切换到long mode */
    
    //setup_page_tables(mem, &sregs);
   // setup_segment_registers(&sregs);
    ioctl(vcpufd, KVM_SET_SREGS, &sregs);   /*  同步我们的设置  */

    
    
    printf("Host pid:%d\n",getpid());
    

    VM VM;
    VM.mem = (void *)mem;
    VM.vcpufd = vcpufd;
    VM.vmfd = vmfd;
    VM.mem_size = MEM_SIZE;
    VM.run = run;
    
     
    while(1){
            //puts("run");
        	ioctl(vcpufd, KVM_RUN, NULL);

            /*  run is a kvm_run *  */
	        switch (run->exit_reason) {
	        
                case KVM_EXIT_IO:
                    /*  用这里来模拟串口的通信  */
                    //printf("run->io.port:%d\n",run->io.port);
                    if(run->io.direction == KVM_EXIT_IO_OUT /*  out  */
                        && run->io.size == 1 
                        && run->io.port == 0x3f8            /*  串口编号   */
                        && run->io.count == 1){
 
                       
                        
                        
                        
                        #ifdef DEBUG
                        printf("run->io.data_offset:0x%llx\n",run->io.data_offset);
                        printf("run:%p\n",run);
                        printf("putchar addr: %p\n",run+run->io.data_offset);
                        #endif
                       // getchar();

                        putchar(
                                *(  ((char*)run) + run->io.data_offset)
                        );

                    }else{
                        errx(1,"Cannot handle this KVM_EXIT_IO");
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
                case KVM_EXIT_SHUTDOWN:
                    errx(1,"KVM_EXIT_SHUTDOWN");
                default:
                    puts("miss!");
                    printf("%d\n",run->exit_reason);
                    return 0;
	    }
    }


}
int main(){
    setbuf(stdout,0);
    setbuf(stdin,0);
    setbuf(stderr,0);
    size_t len;
    const uint8_t code[] = {
	0xba, 0xf8, 0x03, /* mov $0x3f8, %dx */
	0x00, 0xd8,       /* add %bl, %al */
	0x04, '5',        /* add $'0', %al */
	0xee,             /* out %al, (%dx) */
	0xb0, '\n',       /* mov $'\n', %al */
	0xee,             /* out %al, (%dx) */
	0xf4,             /* hlt */
    };

    const uint8_t code_64[]= "H\xb8\x74\x65\x73\x74\x78\x36\x34\nj\bY\xBA\xf8\x03\x00\x00\xEEH\xC1\xE8\b\xE2\xF9\xF4";
    const uint8_t code_64_2[] = {
        0x66,0xba,0xf8,0x03,0x48,0xc7,0xc0,0x12,0x00,0x00,0x00,0xef,0x48,0x65,0x6c,0x6c,0x6f,0x20,0x4b,0x56,0x4d,0x20,0x6b,0x65,0x72,0x6e,0x65,0x6c,0x21,0x0a,
    };
   
   len=31;
   kvm_kernel(code_64,len);
}
