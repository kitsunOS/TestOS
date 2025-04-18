ASM = nasm
ASMFLAGS = -f elf32

TARGET_ARCH := i386
LAYOUT_DIR = layout/$(TARGET_ARCH)
SRC_DIR = arch/$(TARGET_ARCH)
OUT_DIR = out
OBJ_DIR = build

.PHONY: all clean run

all: $(OUT_DIR)/testos.iso

run: all
	qemu-system-$(TARGET_ARCH) -cdrom $(OUT_DIR)/testos.iso -boot d -m 512M -smp 4 -enable-kvm

clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)

$(OUT_DIR)/testos.iso: $(OBJ_DIR)/layout
	@mkdir -p $(dir $@)
	grub-mkrescue -o $@ $<

$(OBJ_DIR)/layout: $(OBJ_DIR)/kernel.bin
	rm -rf $@
	cp $(LAYOUT_DIR) $@ -r
	cp $< $@/boot/testos.bin

$(OBJ_DIR)/kernel.bin: $(OBJ_DIR)/boot.o
	@mkdir -p $(dir $@)
	ld -m elf_$(TARGET_ARCH) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@ -I $(dir $<)