cmake_minimum_required(VERSION 3.23)
project(mini_fps)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/*.cpp)

link_directories("/opt/homebrew/lib")
include_directories("/opt/homebrew/include")

find_package(SDL2 REQUIRED)

add_executable(
        mini_fps
        src/main.cpp
        src/Level.cpp
        src/Level.h
        src/Color.h)

target_link_libraries(
        mini_fps PRIVATE
        /Library/Frameworks/SDL2.framework/Versions/A/SDL2
        /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image
)

target_include_directories(
        mini_fps PRIVATE
        /Library/Frameworks/SDL2.framework/Versions/A/Headers
        /Library/Frameworks/SDL2_image.framework/Versions/A/Headers
)