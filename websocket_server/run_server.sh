#!/bin/bash

echo "========================================"
echo "HIK Camera WebSocket Server"
echo "========================================"
echo ""

if [ ! -f "build/camera_server" ]; then
    echo "错误: 服务器可执行文件未找到!"
    echo "请先运行 ./build.sh"
    exit 1
fi

# 获取本机IP地址
IP=$(hostname -I | awk '{print $1}')

echo "服务器配置:"
echo "- 端口: 9002"
echo "- JPEG质量: 85"
echo "- 帧率: 30 fps"
echo "- 准星: 启用"
echo ""
echo "本机IP地址: $IP"
echo ""
echo "从手机连接:"
echo "  在浏览器中打开: ws://$IP:9002"
echo "  或打开 web_client.html 并修改服务器地址"
echo ""
echo "========================================"
echo "启动服务器..."
echo "========================================"
echo ""

# 设置库路径（如果HIK SDK在非标准位置）
export LD_LIBRARY_PATH=../HIK_TEF_Driver_module/lib:$LD_LIBRARY_PATH

cd build
./camera_server "$@"
