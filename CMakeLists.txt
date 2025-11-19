cmake_minimum_required(VERSION 3.16)

set(TARGET_ARCH i386)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ${TARGET_ARCH})

set(CMAKE_C_COMPILER ${TARGET_ARCH}-elf-gcc)
set(CMAKE_ASM_COMPILER ${TARGET_ARCH}-elf-gcc)
set(CMAKE_AR ${TARGET_ARCH}-elf-ar)
set(CMAKE_RANLIB ${TARGET_ARCH}-elf-ranlib)
set(CMAKE_LINKER ${TARGET_ARCH}-elf-ld)

set(CMAKE_C_FLAGS "-m32 -ffreestanding -nostdlib -Wextra" CACHE STRING "" FORCE)
set(CMAKE_ASM_NASM_FLAGS "-felf32" CACHE STRING "" FORCE)

set(NASM_FLAGS "-felf32")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

option(DEBUG "Enable debug flags" OFF)
if(DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -DDEBUG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

project(kernel LANGUAGES C ASM)

enable_language(ASM_NASM)

set(LAYOUT_SRC_DIR ${CMAKE_SOURCE_DIR}/layout/${TARGET_ARCH})
set(LAYOUT_DEST_DIR ${CMAKE_BINARY_DIR}/layout)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out)
set(KERNEL_ELF ${OUTPUT_DIR}/kernel.elf)
set(ISO_FILE ${OUTPUT_DIR}/testos.iso)

set(KERNEL_COMPILE_OPTIONS
  -mgeneral-regs-only
  -mno-red-zone
  -ffreestanding
  -fno-stack-protector
  -fno-exceptions
)

include_directories(include kernel/include ${CMAKE_SOURCE_DIR})
file(MAKE_DIRECTORY ${OUTPUT_DIR})

add_subdirectory(arch)
add_subdirectory(lib)
add_subdirectory(selftest)
add_subdirectory(drivers)
add_subdirectory(kernel)

add_executable(kernel.elf ${BOOT_PARTIAL_OBJ})
target_link_options(kernel.elf PRIVATE
  -T ${CMAKE_SOURCE_DIR}/arch/${TARGET_ARCH}/linker.ld
  -nostdlib
)

target_link_libraries(kernel.elf PRIVATE arch_objs lib_objs selftest_objs drivers_objs kernel_objs)

add_custom_command(OUTPUT ${KERNEL_ELF}
  COMMAND cp kernel.elf ${KERNEL_ELF}
  DEPENDS kernel.elf
  COMMENT "Generating raw binary..."
)

add_custom_command(OUTPUT ${LAYOUT_DEST_DIR}
  COMMAND rsync -a ${LAYOUT_SRC_DIR}/ ${LAYOUT_DEST_DIR}/
  COMMAND cp ${KERNEL_ELF} ${LAYOUT_DEST_DIR}/boot/testos.elf
  DEPENDS ${KERNEL_ELF}
  COMMENT "Preparing layout directory..."
)

add_custom_target(layout_dir ALL DEPENDS ${LAYOUT_DEST_DIR})

add_custom_command(OUTPUT ${ISO_FILE}
  COMMAND grub-mkrescue -o ${ISO_FILE} ${LAYOUT_DEST_DIR}
  DEPENDS layout_dir
  COMMENT "Creating bootable ISO..."
)

add_custom_target(iso ALL DEPENDS ${ISO_FILE})

add_custom_target(run
  COMMAND qemu-system-${TARGET_ARCH}
    -cdrom ${ISO_FILE}
    -boot d -m 512M -smp 1 -enable-kvm
    -d cpu_reset,unimp,int,guest_errors
    -monitor stdio
    -gdb tcp::6666
  DEPENDS iso
  USES_TERMINAL
)
