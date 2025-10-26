add_subdirectory(input)
add_subdirectory(rescue)

add_library(kernel_objs INTERFACE)

target_sources(kernel_objs INTERFACE
  $<TARGET_OBJECTS:input_objs>
  $<TARGET_OBJECTS:rescue_objs>
)