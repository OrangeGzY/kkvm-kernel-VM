from pwn import *
context.os = 'linux'
context.arch = 'amd64'

code = asm(
f'''
jmp get_str

pop_str: 
    pop rax

{shellcraft.strcpy('rsp','rax')}
{shellcraft.write(1,'rsp',32)}
{shellcraft.exit(0)}

get_str: 
    call pop_str

.asciz "Hello, syscall\\n"

'''
)
run_assembly(code).recvline()
