ASM = nasm
ASMFLAGS = -f elf32

ARCH_OBJ_DIR = $(OBJ_DIR)/arch/$(TARGET_ARCH)

ASM_OBJECTS =

export ASM ASMFLAGS ARCH_OBJ_DIR

.PHONY: FORCE

$(OBJ_DIR)/kernel.bin: $(ARCH_OBJ_DIR)/boot/boot_linked.o $(ASM_OBJECTS) linker.ld
	$(LD) -m elf_$(TARGET_ARCH) -T linker.ld -o $@ $< $(ASM_OBJECTS)

$(ARCH_OBJ_DIR)/boot/boot_linked.o: FORCE
	$(MAKE) ${MFLAGS} -C boot $@

$(ARCH_OBJ_DIR)/%.o: %.asm | $(ARCH_OBJ_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@ -I $(dir $<)

$(ARCH_OBJ_DIR):
	@mkdir -p $(ARCH_OBJ_DIR)

FORCE: ;