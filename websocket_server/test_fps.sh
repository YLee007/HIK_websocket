#!/bin/bash

echo "========================================"
echo "HIK Camera FPS Test"
echo "========================================"
echo ""

# 检查test_camera_fps.cpp是否存在
if [ ! -f "test_camera_fps.cpp" ]; then
    echo "错误: test_camera_fps.cpp 未找到!"
    exit 1
fi

echo "[1/2] 编译测试程序..."

# 编译test_camera_fps (不需要OpenCV)
g++ -o test_camera_fps test_camera_fps.cpp \
    -I../HIK_Camera_C/HIK_TEF_Driver_module \
    -L../HIK_Camera_C/HIK_TEF_Driver_module/lib \
    -lMvCameraControl \
    -lpthread \
    -std=c++11

if [ $? -ne 0 ]; then
    echo "✗ 编译失败"
    exit 1
fi

echo "✓ 编译成功"
echo ""

echo "[2/2] 运行FPS测试..."
echo ""

# 设置库路径
export LD_LIBRARY_PATH=../HIK_Camera_C/HIK_TEF_Driver_module/lib:$LD_LIBRARY_PATH

# 运行测试，过滤十六进制输出
./test_camera_fps 2>&1 | grep -v "^[0-9A-F][0-9A-F]" | grep -v "^[[:space:]]*[0-9A-F][0-9A-F]"

echo ""
echo "========================================"
