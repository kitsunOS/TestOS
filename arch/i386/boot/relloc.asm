extern setup_init_page_map
extern relloc_page_directory
extern stack_start
extern stack_end
extern higher_half_entry

section .text
global rellocate

rellocate:
  call setup_init_page_map

  mov eax, relloc_page_directory
  and eax, 0xFFFFF000
  mov cr3, eax

  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax

  mov esp, stack_end + 0x1000
  mov ebp, stack_end

  jmp higher_half_entry