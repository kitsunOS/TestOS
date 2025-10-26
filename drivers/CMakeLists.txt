add_subdirectory(keyboard)

add_library(drivers_objs INTERFACE)

target_sources(drivers_objs INTERFACE
  $<TARGET_OBJECTS:keyboard_objs>
)