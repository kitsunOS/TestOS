ENTRY(_start)

BOOT_ADDRESS = 0x00100000;
KERNEL_ADDRESS = 0xC0000000;

PHDRS {
  lhtext PT_LOAD FLAGS(5);
  lhdata PT_LOAD FLAGS(6);
  hhtext PT_LOAD FLAGS(5);
  hhdata PT_LOAD FLAGS(6);
}

SECTIONS {
  . = BOOT_ADDRESS;
  __boot_start = .;
  .text.boot ALIGN(8) : {
    *(.text.multiboot)
    *(.text.boot)
  } :lhtext
  .rodata.boot ALIGN(8) : {
    *(.rodata.boot)
  } :lhtext
  .data.boot ALIGN(8) : {
    *(.data.boot)
  } :lhdata
  .bss.boot ALIGN(8) : {
    *(.bss.boot)
  } :lhdata
  . = ALIGN(4096);
  __boot_end = .;

  . = KERNEL_ADDRESS;
  __phys_kernel_start = __boot_end;
  __virt_kernel_start = KERNEL_ADDRESS;
  .text ALIGN(8) : AT(__boot_end) {
    *(.text)
  } :hhtext
  .rodata ALIGN(8) : AT(__boot_end + ADDR(.rodata) - KERNEL_ADDRESS) {
    *(.rodata)
  } :hhtext
  .data ALIGN(8) : AT(__boot_end + ADDR(.data) - KERNEL_ADDRESS) {
    *(.data)
    *(COMMON)
  } :hhdata
  .bss ALIGN(8) : AT(__boot_end + ADDR(.bss) - KERNEL_ADDRESS) {
    *(.bss)
  } :hhdata
  __virt_kernel_end = .;
  __phys_kernel_end = __boot_end + . - KERNEL_ADDRESS;
}