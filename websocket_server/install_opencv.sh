#!/bin/bash

echo "========================================"
echo "OpenCV 安装脚本 for Ubuntu"
echo "========================================"
echo ""

# 检查是否为root
if [ "$EUID" -ne 0 ]; then 
    echo "请使用sudo运行此脚本"
    exit 1
fi

echo "此脚本将安装OpenCV及其依赖"
read -p "是否继续? (y/n) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    exit 1
fi

echo ""
echo "[1/3] 更新软件包列表..."
apt-get update

echo ""
echo "[2/3] 安装OpenCV依赖..."
apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libgtk-3-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libv4l-dev \
    libxvidcore-dev \
    libx264-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    gfortran \
    openexr \
    libatlas-base-dev \
    python3-dev \
    python3-numpy \
    libtbb2 \
    libtbb-dev \
    libdc1394-22-dev

echo ""
echo "[3/3] 安装OpenCV..."
apt-get install -y libopencv-dev

echo ""
echo "========================================"
echo "OpenCV安装完成!"
echo "========================================"
echo ""

# 验证安装
if pkg-config --exists opencv4; then
    echo "OpenCV版本: $(pkg-config --modversion opencv4)"
elif pkg-config --exists opencv; then
    echo "OpenCV版本: $(pkg-config --modversion opencv)"
else
    echo "警告: 无法检测到OpenCV版本"
fi

echo ""
echo "安装完成! 现在可以运行 ./setup_dependencies.sh"
echo ""
