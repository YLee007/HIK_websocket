@echo off
echo ========================================
echo HIK Camera WebSocket Server
echo ========================================
echo.

if not exist "build\Release\camera_server.exe" (
    echo ERROR: Server executable not found!
    echo Please run build.bat first
    pause
    exit /b 1
)

REM 获取本机IP地址
for /f "tokens=2 delims=:" %%a in ('ipconfig ^| findstr /c:"IPv4"') do (
    set IP=%%a
    goto :found
)
:found
set IP=%IP:~1%

echo Server Configuration:
echo - Port: 9002
echo - JPEG Quality: 85
echo - Frame Rate: 30 fps
echo.
echo Your IP Address: %IP%
echo.
echo Connect from your phone:
echo   Open browser and go to: ws://%IP%:9002
echo   Or open web_client.html and change the server address
echo.
echo ========================================
echo Starting server...
echo ========================================
echo.

cd build\Release
camera_server.exe

pause
