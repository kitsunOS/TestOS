ASM = nasm
ASMFLAGS = -f elf32

ARCH_OBJ_DIR = $(OBJ_DIR)/arch/$(TARGET_ARCH)

C_SOURCES = early_print.c hentry.c

OBJECTS_TO_LINK = \
  $(patsubst %.c,$(ARCH_OBJ_DIR)/%.o,$(C_SOURCES))

export ASM ASMFLAGS ARCH_OBJ_DIR

.PHONY: FORCE

$(OBJ_DIR)/kernel.bin: $(ARCH_OBJ_DIR)/boot/boot_linked.o $(OBJECTS_TO_LINK) linker.ld
	$(LD) -m elf_$(TARGET_ARCH) -T linker.ld -o $@ $< $(OBJECTS_TO_LINK)

$(ARCH_OBJ_DIR)/boot/boot_linked.o: FORCE
	$(MAKE) ${MFLAGS} -C boot $@

$(ARCH_OBJ_DIR)/%.o: %.asm | $(ARCH_OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@ -I $(dir $<)

$(ARCH_OBJ_DIR)/%.o: %.c | $(ARCH_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(dir $<)

$(ARCH_OBJ_DIR):
	@mkdir -p $(ARCH_OBJ_DIR)

FORCE: ;