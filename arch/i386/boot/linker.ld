SECTIONS {
  .text.boot ALIGN(8) : {
    *(.text.multiboot)
    *(.text)
  }
  .rodata.boot : {
    *(.rodata)
  }
  .data.boot : {
    *(.data)
  }
  .bss.boot : {
    *(.bss)
  }
}