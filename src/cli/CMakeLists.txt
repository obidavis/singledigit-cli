add_executable(singledigit main.cpp
        generate.cpp
        solve.cpp
        format.cpp
)
include(FetchContent)

include(FetchContent)
FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        v2.2.0
)

FetchContent_MakeAvailable(cli11)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.6
)
FetchContent_MakeAvailable(magic_enum)

target_link_libraries(singledigit PRIVATE CLI11::CLI11 singledigit_lib fmt::fmt nlohmann_json::nlohmann_json magic_enum::magic_enum)

install(TARGETS singledigit)