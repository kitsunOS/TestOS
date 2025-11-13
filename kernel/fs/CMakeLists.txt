add_subdirectory(vfs)

add_library(fs_objs INTERFACE)

target_sources(fs_objs INTERFACE
  $<TARGET_OBJECTS:vfs_objs>
)