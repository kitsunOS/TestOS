%define MB_MAGIC 0xE85250D6
%define MB_ARCH 0x00000000
%define MB_LENGTH (header_end - header_start)
%define MB_CHECKSUM (0 - MB_MAGIC - MB_ARCH - MB_LENGTH)

section .text.multiboot align=8:
header_start:

dd MB_MAGIC
dd MB_ARCH
dd MB_LENGTH
dd MB_CHECKSUM

end_tag_start:
dw 0x0000
dw 0x0000
dd end_tag_end - end_tag_start
end_tag_end:

header_end:
