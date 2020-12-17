from pwn import *
context.arch='amd64'
sc = asm(
"""
    
  movabs rax, 0x0a33323144434241
  push 8
  pop rcx
  mov edx, 0x217
OUT:
  out dx, al
  shr rax, 8
  loop OUT
  hlt

"""
)
print sc
