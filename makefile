TARGET_ARCH := i386
OUT_DIR = out
OBJ_DIR = $(abspath build)
INCLUDE_DIR = $(abspath include)
LAYOUT_SRC_DIR = layout/$(TARGET_ARCH)
LAYOUT_DEST_DIR = $(OBJ_DIR)/layout

CC_PREFIX = i386-elf-
CC = $(CC_PREFIX)gcc
LD = $(CC_PREFIX)ld

CFLAGS = -ffreestanding -fno-stack-protector -m32 -I$(INCLUDE_DIR) -g

LAYOUT_FILES := $(shell find $(LAYOUT_SRC_DIR) -type f)
TARGET_FILES := $(shell find arch/$(TARGET_ARCH) -type f)

LAYOUT_DEPS := $(LAYOUT_FILES) $(OBJ_DIR)/kernel.bin

export TARGET_ARCH OBJ_DIR
export CC CFLAGS LD

.PHONY: all clean run FORCE

all: $(OUT_DIR)/testos.iso

run: all
	qemu-system-$(TARGET_ARCH) -cdrom $(OUT_DIR)/testos.iso -boot d -m 512M -smp 4 -enable-kvm -d cpu_reset,unimp,int,guest_errors -monitor stdio

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

$(OBJ_DIR)/kernel.bin: FORCE | $(OBJ_DIR)
	$(MAKE) ${MFLAGS} -C arch/$(TARGET_ARCH) $@

FORCE: ;