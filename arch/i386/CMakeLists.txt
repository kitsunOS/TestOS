file(GLOB ARCH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE MM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/mm/*.c)
file(GLOB_RECURSE INT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/int/*.c)
set (ARCH_SRC ${ARCH_SRC} ${MM_SRC} ${INT_SRC})

add_subdirectory(boot)

set(BOOT_PARTIAL_OBJ ${CMAKE_CURRENT_BINARY_DIR}/boot/boot_partial.o)
set_source_files_properties(${BOOT_PARTIAL_OBJ} PROPERTIES GENERATED TRUE)

add_library(arch_objs STATIC
  ${ARCH_SRC}
  ${BOOT_PARTIAL_OBJ}
)

add_dependencies(arch_objs boot_obj)
