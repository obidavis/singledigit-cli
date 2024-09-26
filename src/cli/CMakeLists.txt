add_executable(singledigit main.cpp
        output.cpp
        json.cpp
        )
include(FetchContent)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.2.1
)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


FetchContent_MakeAvailable(cxxopts)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++")
target_link_libraries(singledigit PRIVATE cxxopts::cxxopts singledigit_lib fmt::fmt nlohmann_json::nlohmann_json)