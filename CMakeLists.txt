cmake_minimum_required(VERSION 3.28)
project(Gra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable(Gra src/main.cpp
        src/Menu.cpp
        src/Menu.h)
#target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(Gra PRIVATE SFML::Graphics)