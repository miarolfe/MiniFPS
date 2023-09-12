cmake_minimum_required(VERSION 3.23)

project(MiniFPS VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(
        SOURCES
        src/main.cpp
        src/Audio.cpp
        src/Button.cpp
        src/Camera.cpp
        src/Color.cpp
        src/Enemy.cpp
        src/Font.cpp
        src/Game.cpp
        src/Level.cpp
        src/Menu.cpp
        src/Player.cpp
        src/Renderer.cpp
        src/Settings.cpp
        src/Texture.cpp
        src/Utilities.cpp
        src/Vector.cpp
)

set(
        HEADERS
        src/Audio.h
        src/Button.h
        src/Camera.h
        src/Color.h
        src/Enemy.h
        src/Font.h
        src/Game.h
        src/Level.h
        src/Menu.h
        src/Player.h
        src/Point.h
        src/Renderer.h
        src/Settings.h
        src/Texture.h
        src/Utilities.h
        src/Vector.h
)

if(WIN32)
    add_executable(MiniFPS ${SOURCES} ${HEADERS})

    target_link_options(MiniFPS PRIVATE
            -static-libstdc++
            -mwindows
            -static
            -lstdc++
            -lpthread
            )

    target_include_directories(MiniFPS PRIVATE "include/" "win-include/")
    target_link_libraries(MiniFPS PRIVATE ${CMAKE_SOURCE_DIR}/lib/SDL2.dll ${CMAKE_SOURCE_DIR}/lib/SDL2_image.dll ${CMAKE_SOURCE_DIR}/lib/SDL2_mixer.dll ${CMAKE_SOURCE_DIR}/lib/SDL2_ttf.dll)
    add_custom_command(TARGET MiniFPS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows/assets"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2_image.dll" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer.dll" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf.dll" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.exe" "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS-windows"
            )
elseif(APPLE)
    set(SDL2_FRAMEWORK "${PROJECT_SOURCE_DIR}/lib/SDL2.framework")
    set(SDL2_IMAGE_FRAMEWORK "${PROJECT_SOURCE_DIR}/lib/SDL2_image.framework")
    set(SDL2_MIXER_FRAMEWORK "${PROJECT_SOURCE_DIR}/lib/SDL2_mixer.framework")
    set(SDL2_TTF_FRAMEWORK "${PROJECT_SOURCE_DIR}/lib/SDL2_ttf.framework")
    set(CMAKE_BUILD_RPATH "@executable_path/../Frameworks")

    add_executable(MiniFPS MACOSX_BUNDLE ${SOURCES} ${HEADERS})
    set_target_properties(MiniFPS PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/assets/Info.plist
            )

    set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    target_include_directories(MiniFPS PRIVATE "include/")

    target_link_libraries(MiniFPS PRIVATE
            "${SDL2_FRAMEWORK}"
            "${SDL2_IMAGE_FRAMEWORK}"
            "${SDL2_MIXER_FRAMEWORK}"
            "${SDL2_TTF_FRAMEWORK}"
    )

    message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    add_custom_command(TARGET MiniFPS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Frameworks
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2.framework ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Frameworks/SDL2.framework
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image.framework ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Frameworks/SDL2_image.framework
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_mixer.framework ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Frameworks/SDL2_mixer.framework
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf.framework ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Frameworks/SDL2_ttf.framework
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Resources
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/Resources
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/assets/MiniFPS.sh ${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/MacOS

            COMMAND install_name_tool -change ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/MacOS/MiniFPS"
            COMMAND install_name_tool -change ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image.framework/Versions/A/SDL2_image @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/MacOS/MiniFPS"
            COMMAND install_name_tool -change ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_image.framework/Versions/A/SDL2_mixer @executable_path/../Frameworks/SDL2_mixer.framework/Versions/A/SDL2_mixer "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/MacOS/MiniFPS"
            COMMAND install_name_tool -change ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf.framework/Versions/A/SDL2_ttf @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf "${CMAKE_CURRENT_BINARY_DIR}/MiniFPS.app/Contents/MacOS/MiniFPS"
            )
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()