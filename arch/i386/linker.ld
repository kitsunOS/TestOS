ENTRY(_start)

BOOT_ADDRESS = 0x00100000;
KERNEL_ADDRESS = 0xC0000000;

SECTIONS {
  . = BOOT_ADDRESS;
  __boot_start = .;
  .text.boot : {
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
  __kernel_start = __boot_end;
  .text : AT(__boot_end) {
    *(.text)
  }
  .rodata : AT(__boot_end + ADDR(.text) - KERNEL_ADDRESS) {
    *(.rodata)
  }
  .data : AT(__boot_end + ADDR(.rodata) - KERNEL_ADDRESS) {
    *(.data)
  }
  .bss : AT(__boot_end + ADDR(.data) - KERNEL_ADDRESS) {
    *(.bss)
  }
  __kernel_end = __boot_end + . - KERNEL_ADDRESS;

}