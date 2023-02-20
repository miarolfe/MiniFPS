cmake_minimum_required(VERSION 3.23)
project(mini-fps)

include(CPack)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

set(
        SOURCES
        src/main.cpp
        src/Camera.cpp
        src/Level.cpp
)

set(
        HEADERS
        src/Camera.h
        src/Color.h
        src/Level.h
)


link_directories("/opt/homebrew/lib")
include_directories("/opt/homebrew/include")

find_package(SDL2 REQUIRED)

add_executable(mini-fps MACOSX_BUNDLE ${SOURCES} ${HEADERS})

set_target_properties(mini-fps PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "mini-fps"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.pjhrolfe.mini-fps"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
)

target_include_directories(mini-fps PRIVATE
        /Library/Frameworks/SDL2.framework/Versions/A/Headers
        /Library/Frameworks/SDL2_image.framework/Versions/A/Headers
)

target_link_libraries(mini-fps PRIVATE
        /Library/Frameworks/SDL2.framework/Versions/A/SDL2
        /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image
)

add_custom_command(TARGET mini-fps POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
        )
