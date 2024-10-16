set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest EXCLUDE_FROM_ALL)
include(googletest)
enable_testing()

add_subdirectory(singledigit_lib)
