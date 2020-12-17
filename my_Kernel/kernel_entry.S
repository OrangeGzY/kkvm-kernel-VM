.globl _start, hlt
.extern kernel_enter
.intel_syntax noprefix
_start:
    mov rdx, [rsp] /* argc */
    lea rcx, [rsp + 8] /* argv */
    call kernel_enter /* real main */
hlt:
    hlt 
    jmp hlt
    
