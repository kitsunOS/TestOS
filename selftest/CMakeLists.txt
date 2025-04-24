file(GLOB SELFTEST_SRC CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_subdirectory(mm)

add_library(selftest_objs STATIC
  ${SELFTEST_SRC}
  $<TARGET_OBJECTS:selftest_mm_objs>
)