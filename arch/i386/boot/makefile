BOOT_OBJ_DIR = $(ARCH_OBJ_DIR)/boot

ASM_SOURCES = boot.asm relloc.asm
C_SOURCES = rellocc.c

OBJECTS_TO_LINK = \
  $(patsubst %.asm,$(BOOT_OBJ_DIR)/%.o,$(ASM_SOURCES)) \
  $(patsubst %.c,$(BOOT_OBJ_DIR)/%.o,$(C_SOURCES))

$(BOOT_OBJ_DIR)/boot_linked.o: $(OBJECTS_TO_LINK) linker.ld
	$(LD) -r -m elf_$(TARGET_ARCH) -T linker.ld -o $@ $(OBJECTS_TO_LINK)

$(BOOT_OBJ_DIR)/%.o: %.asm | $(BOOT_OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@ -I $(dir $<)

$(BOOT_OBJ_DIR)/%.o: %.c | $(BOOT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(dir $<)

$(BOOT_OBJ_DIR):
	@mkdir -p $(BOOT_OBJ_DIR)