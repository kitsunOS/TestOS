%include "multiboot.inc"

%define FB_ADDR 0xB8000

section .text:
global _start
_start:
  mov eax, 0xB8000
  mov ebx, [boot_msg]
  mov ecx, boot_msg + 4

print_loop:
  cmp ebx, 0
  je done
  mov edx, [ecx]
  mov [eax], edx
  mov byte [eax + 1], 0x07
  add eax, 2
  dec ebx
  add ecx, 1
  jmp print_loop

done:
  hlt
  jmp $

.data:
boot_msg:
  dd 13
  db "Hello, World!"