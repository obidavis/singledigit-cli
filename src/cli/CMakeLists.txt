add_executable(singledigit main.cpp)
include(FetchContent)

FetchContent_Declare(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.2.1
)
FetchContent_MakeAvailable(cxxopts)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++")
target_link_libraries(singledigit PRIVATE cxxopts::cxxopts singledigit_lib fmt::fmt)