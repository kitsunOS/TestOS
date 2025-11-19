file(GLOB INPUT_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(input_objs OBJECT ${INPUT_SRC})

target_compile_options(input_objs PUBLIC ${KERNEL_COMPILE_OPTIONS})