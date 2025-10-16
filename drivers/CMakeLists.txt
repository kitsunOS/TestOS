add_subdirectory(keyboard)

add_library(drivers_objs OBJECT $<TARGET_OBJECTS:keyboard_objs>)