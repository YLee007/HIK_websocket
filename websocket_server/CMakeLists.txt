cmake_minimum_required(VERSION 3.10)
project(HIK_Camera_WebSocket_Server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenCV
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../HIK_TEF_Driver_module
    ${CMAKE_CURRENT_SOURCE_DIR}/../HIK_TEF_Driver_module/include
)

# WebSocket++和Asio头文件路径（需要手动下载）
# 下载地址：
# WebSocket++: https://github.com/zaphoyd/websocketpp
# Asio: https://github.com/chriskohlhoff/asio
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/websocketpp
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/asio/include
)

# 添加HIK SDK库路径
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../HIK_TEF_Driver_module/lib
)

# 源文件
set(SOURCES
    main.cpp
    camera_websocket_server.cpp
    ../HIK_TEF_Driver_module/HIK_TEF_Driver.cpp
)

# 创建可执行文件
add_executable(camera_server ${SOURCES})

# 链接库
target_link_libraries(camera_server
    ${OpenCV_LIBS}
    MvCameraControl  # HIK SDK库
    pthread
)

# Windows特定设置
if(WIN32)
    target_link_libraries(camera_server ws2_32)
endif()

# 编译选项
if(MSVC)
    target_compile_options(camera_server PRIVATE /W4)
else()
    target_compile_options(camera_server PRIVATE -Wall -Wextra)
endif()
