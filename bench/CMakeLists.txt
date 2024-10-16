include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.0
        CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=OFF
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(bitset_combinations bitset_combinations.cpp)
target_link_libraries(bitset_combinations benchmark::benchmark singledigit_lib)

add_executable(generate generate.cpp)
target_link_libraries(generate singledigit_lib benchmark::benchmark)