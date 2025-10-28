file(GLOB SELFTEST_SRC CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_subdirectory(lib)
add_subdirectory(mm)

add_library(selftest_objs INTERFACE)
target_sources(selftest_objs INTERFACE
  ${SELFTEST_SRC}
  $<TARGET_OBJECTS:selftest_mm_objs>
  $<TARGET_OBJECTS:selftest_lib_objs>
)