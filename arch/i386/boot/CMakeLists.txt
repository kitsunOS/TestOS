file(GLOB_RECURSE BOOT_C_SRC CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

file(GLOB_RECURSE BOOT_ASM_SRC CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.asm
)

set_source_files_properties(${BOOT_ASM_SRC} PROPERTIES LANGUAGE ASM_NASM)

add_executable(boot_partial.o ${BOOT_C_SRC} ${BOOT_ASM_SRC})

target_link_options(boot_partial.o PRIVATE
  -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
  -nostdlib -r
)
add_custom_target(boot_obj ALL DEPENDS boot_partial.o)

set(BOOT_PARTIAL_OBJ ${CMAKE_CURRENT_BINARY_DIR}/boot_partial.o PARENT_SCOPE)

target_include_directories(boot_partial.o PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})