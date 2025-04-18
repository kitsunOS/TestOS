TARGET_ARCH := i386
OUT_DIR = out
OBJ_DIR = build
LAYOUT_SRC_DIR = layout/$(TARGET_ARCH)
LAYOUT_DEST_DIR = $(OBJ_DIR)/layout

LAYOUT_FILES := $(shell find $(LAYOUT_SRC_DIR) -type f)
TARGET_FILES := $(shell find arch/$(TARGET_ARCH) -type f)

LAYOUT_DEPS := $(LAYOUT_FILES) $(OBJ_DIR)/kernel.bin 

.PHONY: all clean run

all: $(OUT_DIR)/testos.iso

run: all
	qemu-system-$(TARGET_ARCH) -cdrom $(OUT_DIR)/testos.iso -boot d -m 512M -smp 4 -enable-kvm

clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OUT_DIR)/testos.iso: $(LAYOUT_DEPS) $(LAYOUT_DEST_DIR) | $(OUT_DIR)
	grub-mkrescue -o $@ $(LAYOUT_DEST_DIR)

$(LAYOUT_DEST_DIR): $(LAYOUT_DEPS) | $(OBJ_DIR)
	mkdir -p $(LAYOUT_DEST_DIR)
	rsync -a $(LAYOUT_SRC_DIR)/ $@
	cp $(OBJ_DIR)/kernel.bin $@/boot/testos.bin

$(OBJ_DIR)/kernel.bin: $(OBJ_DIR)/boot.o
	ld -m elf_$(TARGET_ARCH) -T linker.ld -o $@ $<

$(OBJ_DIR)/boot.o: $(TARGET_FILES) | $(OBJ_DIR)
	$(MAKE) ${MFLAGS} -C arch/$(TARGET_ARCH) $(abspath $@) OBJ_DIR=$(abspath $(OBJ_DIR)) TARGET_ARCH=$(TARGET_ARCH)