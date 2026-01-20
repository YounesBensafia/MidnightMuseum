@echo off

echo [1/3] Configuring CMake...
cmake -G "MinGW Makefiles" -S . -B build
if %errorlevel% neq 0 (
    echo Configuration failed!
    pause
    exit /b %errorlevel%
)

echo [2/3] Building project...
cmake --build build
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo [3/3] Copying DLLs...
copy /Y "..\libs\GLFW\glfw-3.4.bin.WIN64\glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll" "build\"
copy /Y "..\libs\assimp-6.0.3\build\bin\libassimp-6.dll" "build\"
copy /Y "..\libs\mingw64\winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2\mingw64\bin\libstdc++-6.dll" "build\" 2>nul
copy /Y "..\libs\mingw64\winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2\mingw64\bin\libgcc_s_seh-1.dll" "build\" 2>nul
copy /Y "..\libs\mingw64\winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2\mingw64\bin\libwinpthread-1.dll" "build\" 2>nul

echo.
echo Build successful!
echo.
echo [4/4] Running application...
echo.
build\main.exe
if %errorlevel% neq 0 (
    echo.
    echo Application crashed or returned error code: %errorlevel%
)
pause

