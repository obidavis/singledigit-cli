set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

add_subdirectory(singledigit_lib)
