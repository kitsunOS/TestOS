%include "multiboot.inc"

extern rellocate

section .text
global _start
_start:
  mov esp, stack_start
  cmp eax, 0x36d76289
  jne print_mb2_issue
  mov [_multiboot2_info], ebx
  jmp rellocate

print_mb2_issue:
  mov edi, 0xB8000
  mov ebx, [mb2_issue_msg]
  mov esi, mb2_issue_msg + 4

print_loop:
  cmp ebx, 0
  je done
  lodsb
  mov ah, 0x07
  stosw
  dec ebx
  jmp print_loop

done:
  cli
  hlt

section .data
mb2_issue_msg:
  dd 34
  db "ERROR: Multiboot2 header not found"

section .bss
global _multiboot2_info
stack_end:
  resb 0x1000
stack_start:

_multiboot2_info:
  resd 1