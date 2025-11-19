file(GLOB MM_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(mm_objs OBJECT ${MM_SRC})

target_compile_options(mm_objs PUBLIC ${KERNEL_COMPILE_OPTIONS})