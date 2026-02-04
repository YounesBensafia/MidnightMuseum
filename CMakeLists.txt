cmake_minimum_required(VERSION 3.15)
project(OpenGL_TP3)

set(CMAKE_CXX_STANDARD 17)

set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${GLM_DIR})

# Add executable
add_executable(main 
    src/main.cpp
    src/Application.cpp
    src/ResourceManager.cpp
    src/MuseumScene.cpp
    src/Room1.cpp
    src/Room2.cpp
    src/Hallway.cpp
    src/fbxload.cpp
    src/audio_playback.cpp
    src/glad.c
    src/shader.cpp
    src/objload.cpp
)

target_link_libraries(main glfw OpenGL::GL assimp::assimp)
