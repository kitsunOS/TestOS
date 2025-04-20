%include "multiboot.inc"

extern rellocate

section .text
global _start
_start:
  mov esp, stack_start

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
  jmp rellocate

.data:
boot_msg:
  dd 6
  db "TestOS"

section .bss
  stack_end:
  resb 0x1000
  stack_start: