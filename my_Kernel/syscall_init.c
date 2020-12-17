#include "syscall_general_entry.S"
#define MSR_STAR 0xc0000081 /* legacy mode SYSCALL target */
#define MSR_LSTAR 0xc0000082 /* long mode SYSCALL target */
#define MSR_CSTAR 0xc0000083 /* compat mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084
#define SYS_INIT_OK 1
/*

change MSR

edx:eax -> wrmsr -> ecx(MSR)

*/
int syscall_init(){
asm(

".intel_syntax noprefix;"
"xor rax, rax;"
"mov rdx, 0x00200008;"
"mov ecx, 0xc0000081;"
"wrmsr;"

"xor rdx, rdx;"
"mov eax, 0x3f7fd5;"
"mov ecx, 0xc0000084;"
"wrmsr;"

/*use MSR_LSTAR to set kernel RIP */
"lea rdi, [rip+syscall_general_entry];"   // when we switch to the kernel , we will get to syscall_general_handler!
/*the following 3 instructions make the value of rdi to edx:eax*/
"mov rdx, rdi;"
"mov eax, edi;" /* mov low 32bits in rdi to eax */
"shr rdx, 32;"  /* mov high 32bits in rdx(rdi) to low 32bits */
"mov ecx, 0xc0000082;"
"wrmsr;"

);

return SYS_INIT_OK; 
}
