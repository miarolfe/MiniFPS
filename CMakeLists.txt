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

if(WIN32)
    add_executable(mini-fps ${SOURCES} ${HEADERS})
    target_include_directories(mini-fps PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(mini-fps PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
elseif(APPLE)
    # Find SDL2 and SDL2_image frameworks
    find_library(SDL2_LIB SDL2)
    find_library(SDL2_IMAGE_LIB SDL2_image)

    # Bundle frameworks with the app
    set(SDL2_FRAMEWORK "${SDL2_LIB}")
    set(SDL2_IMAGE_FRAMEWORK "${SDL2_IMAGE_LIB}")
    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")

    add_executable(mini-fps MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    include_directories("include")

    target_link_libraries(mini-fps PRIVATE
            "-F/Library/Frameworks"
            "${SDL2_FRAMEWORK}"
            "${SDL2_IMAGE_FRAMEWORK}"
    )

    add_custom_command(TARGET mini-fps POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks"
            COMMAND install_name_tool -change /Library/Frameworks/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/MacOS/mini-fps"
            COMMAND install_name_tool -change /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/MacOS/mini-fps"
            COMMAND ${CMAKE_COMMAND} -E copy_directory /Library/Frameworks/SDL2.framework "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks/SDL2.framework"
            COMMAND ${CMAKE_COMMAND} -E copy_directory /Library/Frameworks/SDL2_image.framework "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks/SDL2_image.framework"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/testIcon.icns" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents"
            )
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()