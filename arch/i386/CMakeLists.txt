file(GLOB ARCH_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE MM_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mm/*.c)
file(GLOB_RECURSE INT_C_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/int/*.c)

set_source_files_properties(${INT_ASM_SRC} PROPERTIES LANGUAGE ASM_NASM)
file(GLOB_RECURSE INT_ASM_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/int/*.asm)

set(ARCH_SRC ${ARCH_SRC} ${MM_SRC} ${INT_C_SRC})

add_subdirectory(boot)

set(BOOT_PARTIAL_OBJ ${CMAKE_CURRENT_BINARY_DIR}/boot/boot_partial.o)
set_source_files_properties(${BOOT_PARTIAL_OBJ} PROPERTIES GENERATED TRUE)

add_library(arch_c_objs STATIC
  ${ARCH_SRC}
  ${BOOT_PARTIAL_OBJ}
)

target_compile_options(arch_c_objs PRIVATE ${KERNEL_COMPILE_OPTIONS})

add_dependencies(arch_c_objs boot_obj)

add_library(arch_objs OBJECT ${INT_ASM_SRC})
target_link_libraries(arch_objs arch_c_objs)