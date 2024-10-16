set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest EXCLUDE_FROM_ALL)
include(googletest)
enable_testing()

add_subdirectory(singledigit_lib)

#add_executable(test_cli_generate test_cli_generate.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/../src/cli/generate.cpp
#)
#target_include_directories(test_cli_generate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/cli)
#target_link_libraries(test_cli_generate PRIVATE singledigit_lib gtest_main gmock)
#gtest_discover_tests(test_cli_generate)
#message(STATUS "Adding test test_cli_generate")
