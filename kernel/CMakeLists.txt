add_subdirectory(input)

add_library(kernel_objs OBJECT $<TARGET_OBJECTS:input_objs>)