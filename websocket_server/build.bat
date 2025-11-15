@echo off
echo ========================================
echo Building HIK Camera WebSocket Server
echo ========================================
echo.

REM 检查依赖
if not exist "third_party\websocketpp" (
    echo ERROR: WebSocket++ not found!
    echo Please run setup_dependencies.bat first
    pause
    exit /b 1
)

if not exist "third_party\asio" (
    echo ERROR: Asio not found!
    echo Please run setup_dependencies.bat first
    pause
    exit /b 1
)

REM 创建build目录
if not exist "build" mkdir build
cd build

echo Configuring CMake...
cmake .. -G "Visual Studio 16 2019"
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed
    echo Please check if CMake and Visual Studio are installed
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Executable location: build\Release\camera_server.exe
echo.
echo To run the server:
echo   cd build\Release
echo   camera_server.exe
echo.
pause
