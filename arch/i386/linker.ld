ENTRY(_start)

BOOT_ADDRESS = 0x00100000;
KERNEL_ADDRESS = 0xC0000000;

SECTIONS {
  . = BOOT_ADDRESS;
  __boot_start = .;
  .text.boot ALIGN(8) : {
    *(.text.multiboot)
    *(.text.boot)
  }
  .rodata.boot : {
    *(.rodata.boot)
  }
  .data.boot : {
    *(.data.boot)
  }
  .bss.boot : {
    *(.bss.boot)
  }
  __boot_end = .;

  . = KERNEL_ADDRESS;
  __phys_kernel_start = __boot_end;
  __virt_kernel_start = KERNEL_ADDRESS;
  .text : AT(__boot_end) {
    *(.text)
  }
  .rodata : AT(__boot_end + ADDR(.rodata) - KERNEL_ADDRESS) {
    *(.rodata)
  }
  .data : AT(__boot_end + ADDR(.data) - KERNEL_ADDRESS) {
    *(.data)
    *(COMMON)
  }
  .bss : AT(__boot_end + ADDR(.bss) - KERNEL_ADDRESS) {
    *(.bss)
  }
  __virt_kernel_end = .;
  __phys_kernel_end = __boot_end + . - KERNEL_ADDRESS;

}