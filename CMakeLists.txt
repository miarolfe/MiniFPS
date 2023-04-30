cmake_minimum_required(VERSION 3.23)

project(mini-fps VERSION 0.0.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(
        SOURCES
        src/main.cpp
        src/Camera.cpp
        src/Level.cpp
        src/Renderer.cpp
        src/Player.cpp
        src/Utilities.cpp
)

set(
        HEADERS
        src/Camera.h
        src/Color.h
        src/Level.h
        src/Player.h
        src/Renderer.h
        src/Settings.h
        src/Utilities.h
)

find_library(SDL2_LIBRARY SDL2)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message(STATUS "SDL2 library found at ${SDL2_LIBRARY}")
else()
    message(FATAL_ERROR "SDL2 library not found")
endif()

find_package(SDL2_image REQUIRED SDL2)
if(SDL2_IMAGE_FOUND)
    message(STATUS "SDL2_image library found at ${SDL2_IMAGE_LIBRARIES}")
else()
    message(FATAL_ERROR "SDL2_image library not found")
endif()

if(WIN32)
    add_executable(mini-fps ${SOURCES} ${HEADERS})
    target_include_directories(mini-fps PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(mini-fps PRIVATE SDL2)
elseif(APPLE)
    add_executable(mini-fps MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    target_include_directories(mini-fps PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} "include/" "/usr/local" "/opt/homebrew")
    target_link_libraries(mini-fps PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

    add_custom_command(TARGET mini-fps POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/testIcon.icns" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents"
            )
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()