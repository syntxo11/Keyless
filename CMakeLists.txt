cmake_minimum_required(VERSION 3.20)
project(Keyless VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(KEYLESS_BUILD_GAME "Build the SFML game executable" ON)

include(CTest)

add_executable(KeylessLogicTests tests/gameplay_utils_tests.cpp)
target_include_directories(KeylessLogicTests PRIVATE include)
add_test(NAME gameplay_utils COMMAND KeylessLogicTests)

if(KEYLESS_BUILD_GAME)
    include(FetchContent)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SFML)

    add_executable(Keyless
        src/main.cpp
        src/Entity.cpp
        src/Player.cpp
        src/Enemy.cpp
        src/Hud.cpp
        src/GameUI.cpp
    )
    target_include_directories(Keyless PRIVATE include)
    target_link_libraries(Keyless PRIVATE sfml-graphics)

    if(MSVC)
        target_compile_options(Keyless PRIVATE /W4 /permissive-)
    else()
        target_compile_options(Keyless PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_custom_command(
        TARGET Keyless POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:Keyless>/assets"
        COMMENT "Copying runtime assets"
    )
endif()
