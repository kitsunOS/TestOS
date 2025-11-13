add_subdirectory(input)
add_subdirectory(rescue)
add_subdirectory(fs)
add_subdirectory(mm)

add_library(kernel_objs INTERFACE)

target_sources(kernel_objs INTERFACE
  $<TARGET_OBJECTS:input_objs>
  $<TARGET_OBJECTS:rescue_objs>
  $<TARGET_OBJECTS:mm_objs>
)

target_link_libraries(kernel_objs INTERFACE fs_objs)