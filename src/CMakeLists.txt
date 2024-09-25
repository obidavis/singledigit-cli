include(FetchContent)
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
        )
FetchContent_MakeAvailable(fmt)
target_compile_definitions(fmt PUBLIC FMT_HEADER_ONLY)

add_subdirectory(cli)
add_subdirectory(lib)
