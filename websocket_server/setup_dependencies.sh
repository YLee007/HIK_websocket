#!/bin/bash

echo "========================================"
echo "HIK Camera WebSocket Server - Setup"
echo "========================================"
echo ""

# 检查是否为root用户
if [ "$EUID" -eq 0 ]; then 
    echo "警告: 不建议使用root用户运行此脚本"
    read -p "是否继续? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# 创建third_party目录
if [ ! -d "third_party" ]; then
    mkdir -p third_party
fi
cd third_party

echo "[1/4] 检查系统依赖..."

# 检查git
if ! command -v git &> /dev/null; then
    echo "Git未安装，正在安装..."
    sudo apt-get update
    sudo apt-get install -y git
fi

# 检查cmake
if ! command -v cmake &> /dev/null; then
    echo "CMake未安装，正在安装..."
    sudo apt-get install -y cmake
fi

# 检查g++
if ! command -v g++ &> /dev/null; then
    echo "G++未安装，正在安装..."
    sudo apt-get install -y build-essential
fi

# 检查OpenCV
echo "[2/4] 检查OpenCV..."
if ! pkg-config --exists opencv4 && ! pkg-config --exists opencv; then
    echo "OpenCV未安装，正在安装..."
    sudo apt-get install -y libopencv-dev
else
    echo "OpenCV已安装"
fi

echo ""
echo "[3/4] 下载WebSocket++..."
if [ ! -d "websocketpp" ]; then
    git clone https://github.com/zaphoyd/websocketpp.git
    if [ $? -ne 0 ]; then
        echo "错误: 下载WebSocket++失败"
        echo "请手动下载: https://github.com/zaphoyd/websocketpp"
        exit 1
    fi
    echo "WebSocket++下载成功!"
else
    echo "WebSocket++已存在，跳过..."
fi

echo ""
echo "[4/4] 下载Asio..."
if [ ! -d "asio" ]; then
    git clone https://github.com/chriskohlhoff/asio.git
    if [ $? -ne 0 ]; then
        echo "错误: 下载Asio失败"
        echo "请手动下载: https://github.com/chriskohlhoff/asio"
        exit 1
    fi
    echo "Asio下载成功!"
else
    echo "Asio已存在，跳过..."
fi

cd ..

echo ""
echo "========================================"
echo "安装完成!"
echo "========================================"
echo ""
echo "下一步:"
echo "1. 确保HIK SDK已安装"
echo "2. 运行 ./build.sh 编译项目"
echo ""
