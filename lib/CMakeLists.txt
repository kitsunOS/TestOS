file(GLOB LIB_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(lib_objs OBJECT ${LIB_SRC})

target_compile_options(lib_objs PUBLIC ${KERNEL_COMPILE_OPTIONS})