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

echo [3/3] Copying DLL...
copy /Y "..\libs\GLFW\glfw-3.4.bin.WIN64\glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll" "build\"

echo.
echo Build successful!
echo Run: build\main.exe ...
build\main.exe
pause

