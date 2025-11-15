@echo off
echo ========================================
echo HIK Camera WebSocket Server - Setup
echo ========================================
echo.

REM 创建third_party目录
if not exist "third_party" mkdir third_party
cd third_party

echo [1/2] Downloading WebSocket++...
if not exist "websocketpp" (
    git clone https://github.com/zaphoyd/websocketpp.git
    if %errorlevel% neq 0 (
        echo ERROR: Failed to download WebSocket++
        echo Please download manually from: https://github.com/zaphoyd/websocketpp
        pause
        exit /b 1
    )
    echo WebSocket++ downloaded successfully!
) else (
    echo WebSocket++ already exists, skipping...
)

echo.
echo [2/2] Downloading Asio...
if not exist "asio" (
    git clone https://github.com/chriskohlhoff/asio.git
    if %errorlevel% neq 0 (
        echo ERROR: Failed to download Asio
        echo Please download manually from: https://github.com/chriskohlhoff/asio
        pause
        exit /b 1
    )
    echo Asio downloaded successfully!
) else (
    echo Asio already exists, skipping...
)

cd ..

echo.
echo ========================================
echo Setup completed successfully!
echo ========================================
echo.
echo Next steps:
echo 1. Make sure OpenCV is installed
echo 2. Run build.bat to compile the project
echo.
pause
