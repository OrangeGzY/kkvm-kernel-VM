.global syscall_general_entry, kernel_stack, user_stack
.extern do_syscall
.intel_syntax noprefix

kernel_stack: .quad 0
user_stack: .quad 0


syscall_general_entry:
    /* attention! we have to protect rsp by ourself instead of "syscall" */
    mov [rip+user_stack], rsp
    mov rsp, [rip+kernel_stack]
    
    /*save user registers*/
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11

    mov rcx, r10
    call do_syscall

    pop r11
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi

    mov rsp, [rip+user_stack]
    .byte 0x48  /* ensure that sysret is 64bits */
    sysret    




