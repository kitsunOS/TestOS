extern init_identity_paging
extern page_directory

section .text
global rellocate

rellocate:
  call init_identity_paging

  mov eax, page_directory
  and eax, 0xFFFFF000
  mov cr3, eax

  mov eax, cr4
  and eax, 0x11111101
  mov cr4, eax

  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax

  mov cl, 'P'
  mov [0xB8000], cl
  hlt
  jmp $
  