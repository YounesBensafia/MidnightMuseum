cmake_minimum_required(VERSION 3.15)

# Set compilers BEFORE project() - using CACHE FORCE
set(CMAKE_C_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/../libs/mingw64/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2/mingw64/bin/gcc.exe" CACHE FILEPATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/../libs/mingw64/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2/mingw64/bin/g++.exe" CACHE FILEPATH "C++ compiler" FORCE)
set(CMAKE_MAKE_PROGRAM "${CMAKE_CURRENT_SOURCE_DIR}/../libs/mingw64/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2/mingw64/bin/mingw32-make.exe" CACHE FILEPATH "Make program" FORCE)

project(OpenGL_TP3)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/GLFW/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64")
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs")
set(ASSIMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/assimp-6.0.3/build")

# Include directories for GLFW, GLAD, GLM, and Assimp
include_directories(${GLFW_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${GLM_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libs/assimp-6.0.3/include)
include_directories(${ASSIMP_DIR}/include)  # For generated config.h

link_directories(${GLFW_DIR}/lib-mingw-w64)
link_directories(${ASSIMP_DIR}/lib)

# Add executable with main.cpp and glad.c
add_executable(main main.cpp src/Application.cpp src/ResourceManager.cpp src/MuseumScene.cpp src/fbxload.cpp src/glad.c shader/shader/shader.cpp model/objload.cpp)

target_link_libraries(main glfw3 opengl32 gdi32 assimp)