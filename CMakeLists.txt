cmake_minimum_required(VERSION 3.23)
project(mini-fps LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

include_directories("include/")
# TODO: Copy changed assets even with no code change

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

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")

    # Find SDL2 and SDL2_image frameworks
    find_library(SDL2_LIB SDL2)
    find_library(SDL2_IMAGE_LIB SDL2_image)

    # Bundle frameworks with the app
    set(SDL2_FRAMEWORK "${SDL2_LIB}")
    set(SDL2_IMAGE_FRAMEWORK "${SDL2_IMAGE_LIB}")
    set(MACOSX_BUNDLE_BUNDLE_NAME "mini-fps")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.pjhrolfe.mini-fps")
    set(MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
    set(MACOSX_BUNDLE_ICON_FILE "testIcon.icns")

    set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    # Add the executable
    add_executable(mini-fps MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${ICON})

    # Set target properties
    set_target_properties(mini-fps PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "${MACOSX_BUNDLE_BUNDLE_NAME}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "${MACOSX_BUNDLE_GUI_IDENTIFIER}"
            MACOSX_BUNDLE_INFO_PLIST "${MACOSX_BUNDLE_INFO_PLIST}"
            MACOSX_BUNDLE_ICON_FILE "${MACOSX_BUNDLE_ICON_FILE}"
            )

    # Include directories for SDL2 and SDL2_image
    include_directories("/Library/Frameworks/SDL2.framework/Headers")
    include_directories("/Library/Frameworks/SDL2_image.framework/Headers")

    # Link with SDL2 and SDL2_image frameworks
    target_link_libraries(mini-fps
            "-F/Library/Frameworks"
            "${SDL2_FRAMEWORK}"
            "${SDL2_IMAGE_FRAMEWORK}"
            )

    # Copy assets to app bundle
    add_custom_command(TARGET mini-fps POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks"
            COMMAND install_name_tool -change /Library/Frameworks/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/MacOS/mini-fps"
            COMMAND install_name_tool -change /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/MacOS/mini-fps"
            COMMAND ${CMAKE_COMMAND} -E copy_directory /Library/Frameworks/SDL2.framework "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks/SDL2.framework"
            COMMAND ${CMAKE_COMMAND} -E copy_directory /Library/Frameworks/SDL2_image.framework "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Frameworks/SDL2_image.framework"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/testIcon.icns" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents/Resources"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist" "${CMAKE_CURRENT_BINARY_DIR}/mini-fps.app/Contents"
            )
endif()

if(WIN32)
    add_executable(mini-fps ${SOURCES} ${HEADERS})


    target_link_libraries(mini-fps
            "${CMAKE_SOURCE_DIR}/lib/SDL2.dll"
            "${CMAKE_SOURCE_DIR}/lib/SDL2_image.dll"
            )

    target_link_options(mini-fps PRIVATE
            -static-libstdc++
            -mwindows
            -static
            -lstdc++
            -lpthread
    )

    include_directories("${CMAKE_SOURCE_DIR}/include")
    add_custom_command(TARGET mini-fps POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:mini-fps>/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCES_DIR}" "$<TARGET_FILE_DIR:mini-fps>/assets"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2.dll" "$<TARGET_FILE_DIR:mini-fps>"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2_image.dll" "$<TARGET_FILE_DIR:mini-fps>"
            )
endif()