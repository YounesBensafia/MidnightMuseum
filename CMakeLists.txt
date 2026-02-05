cmake_minimum_required(VERSION 3.15)
project(OpenGL_TP3)

set(CMAKE_CXX_STANDARD 17)

set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${GLM_DIR})

# Add executable
add_executable(main
    src/main.cpp
    src/core/Application.cpp
    src/core/ResourceManager.cpp
    src/core/shader.cpp
    src/core/audio_playback.cpp
    src/model/fbxload.cpp
    src/model/objload.cpp
    src/scene/MuseumScene.cpp
    src/scene/Room1.cpp
    src/scene/Room2.cpp
    src/scene/Hallway.cpp
    external/glad/glad.c
)

target_link_libraries(main glfw OpenGL::GL assimp::assimp)
