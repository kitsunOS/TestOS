file(GLOB RESCUE_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(rescue_objs OBJECT ${RESCUE_SRC})

target_compile_options(rescue_objs PUBLIC ${KERNEL_COMPILE_OPTIONS})