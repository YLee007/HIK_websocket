#!/bin/bash

echo "========================================"
echo "HIK Camera Test Program"
echo "========================================"
echo ""

# 检查test_camera.cpp是否存在
if [ ! -f "test_camera.cpp" ]; then
    echo "错误: test_camera.cpp 未找到!"
    exit 1
fi

echo "[1/3] 编译test_camera..."

# 编译test_camera
g++ -o test_camera test_camera.cpp \
    -I../HIK_Camera_C/HIK_TEF_Driver_module \
    -L../HIK_Camera_C/HIK_TEF_Driver_module/lib \
    -lMvCameraControl \
    $(pkg-config --cflags --libs opencv4) \
    -lpthread \
    -std=c++11

if [ $? -ne 0 ]; then
    echo "✗ 编译失败"
    echo ""
    echo "请确保:"
    echo "  1. OpenCV已安装: sudo apt install libopencv-dev"
    echo "  2. HIK SDK已安装"
    echo "  3. 路径正确"
    exit 1
fi

echo "✓ 编译成功"
echo ""

echo "[2/3] 设置库路径..."
export LD_LIBRARY_PATH=../HIK_Camera_C/HIK_TEF_Driver_module/lib:$LD_LIBRARY_PATH
echo "✓ 库路径已设置"
echo ""

echo "[3/3] 运行测试程序..."
echo ""

# 运行test_camera，过滤十六进制输出
./test_camera 2>&1 | grep -E "^=|^✓|^✗|^HIK|^\[|^  |^Please|^Possible|Test completed"

echo ""
echo "========================================"
echo "测试完成"
echo "========================================"
echo ""

# 检查是否生成了图像文件
if [ -f "test_image_rgb2bgr.jpg" ]; then
    echo "✓ 生成的图像文件:"
    ls -lh test_image_*.jpg 2>/dev/null
    echo ""
    echo "请检查这些图像文件，确定哪个颜色正确"
else
    echo "⚠ 未生成图像文件，请检查错误信息"
fi
