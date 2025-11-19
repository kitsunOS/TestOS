file(GLOB FS_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(fs_objs OBJECT ${FS_SRC})

target_compile_options(fs_objs PUBLIC ${KERNEL_COMPILE_OPTIONS})