cmake_minimum_required(VERSION 3.22.1)

enable_language(C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("echo")

include(FetchContent)

FetchContent_Declare(
    spdlog
    URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.14.0.zip"
)
FetchContent_MakeAvailable(spdlog)

add_library(echo SHARED echo.cpp)
target_include_directories(echo PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(echo PRIVATE spdlog::spdlog)
