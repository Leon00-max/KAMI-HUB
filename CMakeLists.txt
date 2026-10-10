cmake_minimum_required(VERSION 3.20)
project(ArcadeHub LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

find_package(SFML 2.6.1 COMPONENTS graphics window system QUIET)

if(NOT SFML_FOUND)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SFML)
endif()

option(KAMI_BUILD_TESTS "Build unit tests" ON)

# Game rules: no SFML dependency, so they build and test anywhere
add_library(tetris_core STATIC src/games/tetris/Tetris.cpp)
target_include_directories(tetris_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tetris_core PUBLIC cxx_std_20)

add_executable(arcade_hub
    src/main.cpp
    src/core/Application.cpp
    src/managers/ResourceManager.cpp
    src/scenes/HomeScene.cpp
    src/ui/Button.cpp
    src/ui/DrawingUtilities.cpp
    src/ui/AnimatedButton.cpp
    src/ui/NavigationBar.cpp
    src/ui/GameCard.cpp
    src/ui/GameBoard.cpp
    src/ui/GameSelectionPanel.cpp
    src/ui/InfoPanel.cpp
    src/ui/GlassPanel.cpp
    src/ui/ActionButtonBar.cpp
)

target_include_directories(arcade_hub PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(arcade_hub PRIVATE cxx_std_20)

foreach(target arcade_hub tetris_core)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

# Copy assets next to the executable so it runs from any working directory
add_custom_command(
    TARGET arcade_hub
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:arcade_hub>/assets
)

if(KAMI_BUILD_TESTS)
    enable_testing()
    add_executable(tetris_tests tests/tetris_tests.cpp)
    target_link_libraries(tetris_tests PRIVATE tetris_core)
    add_test(NAME tetris_tests COMMAND tetris_tests)
endif()

target_link_libraries(arcade_hub PRIVATE tetris_core sfml-graphics sfml-window sfml-system)

if(WIN32 AND SFML_FOUND AND DEFINED SFML_DIR)
    get_filename_component(SFML_ROOT "${SFML_DIR}/../../.." ABSOLUTE)
    file(GLOB SFML_RUNTIME_DLLS "${SFML_ROOT}/bin/*.dll")
    if(SFML_RUNTIME_DLLS)
        add_custom_command(
            TARGET arcade_hub
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${SFML_RUNTIME_DLLS}
                    $<TARGET_FILE_DIR:arcade_hub>
        )
    endif()
endif()
