    .section .data
message:
    .ascii "Hello KVM kernel!\n"
    length = . - message

    .section .text
    .global _start  # must be declare for linker

_start:
    mov $0x3f8, %edx
    mov $2, %ebx
    

