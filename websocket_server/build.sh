#!/bin/bash

echo "========================================"
echo "Building HIK Camera WebSocket Server"
echo "========================================"
echo ""

# 检查依赖
if [ ! -d "third_party/websocketpp" ]; then
    echo "错误: WebSocket++未找到!"
    echo "请先运行 ./setup_dependencies.sh"
    exit 1
fi

if [ ! -d "third_party/asio" ]; then
    echo "错误: Asio未找到!"
    echo "请先运行 ./setup_dependencies.sh"
    exit 1
fi

# 创建build目录
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

echo "配置CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo "错误: CMake配置失败"
    echo "请检查CMake和依赖是否正确安装"
    exit 1
fi

echo ""
echo "编译项目..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "错误: 编译失败"
    exit 1
fi

cd ..

echo ""
echo "========================================"
echo "编译完成!"
echo "========================================"
echo ""
echo "可执行文件位置: build/camera_server"
echo ""
echo "运行服务器:"
echo "  ./run_server.sh"
echo "或者:"
echo "  cd build"
echo "  ./camera_server"
echo ""
