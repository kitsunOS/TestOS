ASM = nasm
ASMFLAGS = -f elf32

$(OBJ_DIR)/%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@ -I $(dir $<)