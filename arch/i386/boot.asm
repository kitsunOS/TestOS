%include "multiboot.inc"

section .text:
global _start
_start:
  mov edi, 0xB8000
  mov ebx, [boot_msg]
  mov esi, boot_msg + 4

print_loop:
  cmp ebx, 0
  je done
  lodsb
  mov ah, 0x07
  stosw
  dec ebx
  jmp print_loop

done:
  hlt
  jmp $

.data:
boot_msg:
  dd 13
  db "Hello, World!"