.globl _start
    .code16
_start:
    mov $0x3f8, %dx ;0x3f8赋给dx
    add %bl, %al    ;al = al+bl
    add $'0', %al   ;
    out %al, (%dx)  ;输出al到0x3f8端口
    mov $'\n', %al  ;al = '\n'
    out %al, (%dx)  ;输出al到0x3f8端口
    hlt             ;虚拟机退出