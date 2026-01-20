@echo off
echo Building Assimp with MinGW...

set MINGW_PATH=C:\Users\ABDOU\Desktop\Projet_SIM\libs\mingw64\winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2\mingw64\bin

mkdir build
cd build

cmake -G "MinGW Makefiles" ^
  -DCMAKE_C_COMPILER="%MINGW_PATH%\gcc.exe" ^
  -DCMAKE_CXX_COMPILER="%MINGW_PATH%\g++.exe" ^
  -DCMAKE_MAKE_PROGRAM="%MINGW_PATH%\mingw32-make.exe" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DASSIMP_BUILD_TESTS=OFF ^
  -DASSIMP_BUILD_ASSIMP_TOOLS=OFF ^
  -DBUILD_SHARED_LIBS=ON ^
  ..

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Building...
mingw32-make -j8

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Build successful!
echo DLL will be in: build\bin\
echo LIB will be in: build\lib\
pause
