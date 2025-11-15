# Ubuntu系统安装指南

本指南专门针对Ubuntu系统（18.04, 20.04, 22.04）的安装和使用说明。

## 快速开始（3步）

### 步骤1：安装依赖

```bash
# 赋予脚本执行权限
chmod +x *.sh

# 安装系统依赖和下载第三方库
./setup_dependencies.sh
```

### 步骤2：编译项目

```bash
./build.sh
```

### 步骤3：运行服务器

```bash
./run_server.sh
```

## 详细安装步骤

### 1. 系统要求

- Ubuntu 18.04 / 20.04 / 22.04 LTS
- 至少2GB RAM
- 网络连接
- sudo权限

### 2. 安装OpenCV（如果未安装）

#### 方法1：使用安装脚本（推荐）

```bash
sudo ./install_opencv.sh
```

#### 方法2：手动安装

```bash
sudo apt-get update
sudo apt-get install -y libopencv-dev
```

#### 方法3：从源码编译（最新版本）

```bash
# 安装依赖
sudo apt-get install -y build-essential cmake git pkg-config
sudo apt-get install -y libgtk-3-dev libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y libjpeg-dev libpng-dev libtiff-dev

# 下载OpenCV
cd ~
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build && cd build

# 编译（需要较长时间）
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
```

### 3. 安装HIK SDK

```bash
# 下载HIK MVS SDK for Linux
# 从海康官网下载: https://www.hikrobotics.com/cn/machinevision/service/download

# 解压并安装
tar -zxvf MVS-*.tar.gz
cd MVS-*
sudo ./install.sh

# 验证安装
ls /opt/MVS/lib
```

### 4. 设置库路径

编辑 `~/.bashrc` 或 `~/.profile`：

```bash
# 添加HIK SDK库路径
export LD_LIBRARY_PATH=/opt/MVS/lib:$LD_LIBRARY_PATH

# 如果SDK在其他位置，修改为实际路径
# export LD_LIBRARY_PATH=/path/to/HIK_SDK/lib:$LD_LIBRARY_PATH
```

使配置生效：

```bash
source ~/.bashrc
```

### 5. 下载WebSocket库

```bash
./setup_dependencies.sh
```

这个脚本会自动：
- 检查并安装git、cmake、g++
- 检查并安装OpenCV
- 下载WebSocket++
- 下载Asio

### 6. 编译项目

```bash
./build.sh
```

编译成功后，可执行文件位于 `build/camera_server`

### 7. 运行服务器

```bash
./run_server.sh
```

或者手动运行：

```bash
cd build
./camera_server [端口] [质量] [帧率] [准星]

# 示例
./camera_server 9002 85 30 1
```

## 脚本说明

### setup_dependencies.sh
自动安装系统依赖并下载第三方库

**功能：**
- 检查并安装git、cmake、g++
- 检查并安装OpenCV
- 下载WebSocket++
- 下载Asio

**使用：**
```bash
chmod +x setup_dependencies.sh
./setup_dependencies.sh
```

### build.sh
编译项目

**功能：**
- 检查依赖是否完整
- 运行CMake配置
- 使用make编译（多核并行）

**使用：**
```bash
chmod +x build.sh
./build.sh
```

### run_server.sh
运行服务器

**功能：**
- 检查可执行文件
- 显示本机IP地址
- 设置库路径
- 启动服务器

**使用：**
```bash
chmod +x run_server.sh
./run_server.sh

# 或带参数
./run_server.sh 9002 85 30 1
```

### install_opencv.sh
安装OpenCV及其依赖

**使用：**
```bash
chmod +x install_opencv.sh
sudo ./install_opencv.sh
```

## 权限设置

所有脚本需要执行权限：

```bash
# 一次性设置所有脚本权限
chmod +x *.sh

# 或单独设置
chmod +x setup_dependencies.sh
chmod +x build.sh
chmod +x run_server.sh
chmod +x install_opencv.sh
```

## 相机权限配置

如果遇到相机权限问题：

```bash
# 添加当前用户到video组
sudo usermod -a -G video $USER

# 重新登录使配置生效
# 或使用：
newgrp video
```

## 防火墙配置

### UFW防火墙

```bash
# 允许端口9002
sudo ufw allow 9002/tcp

# 检查状态
sudo ufw status
```

### iptables

```bash
# 允许端口9002
sudo iptables -A INPUT -p tcp --dport 9002 -j ACCEPT

# 保存规则
sudo iptables-save > /etc/iptables/rules.v4
```

## 开机自启动

### 方法1：使用systemd

创建服务文件 `/etc/systemd/system/hik-camera-server.service`：

```ini
[Unit]
Description=HIK Camera WebSocket Server
After=network.target

[Service]
Type=simple
User=your_username
WorkingDirectory=/path/to/websocket_server/build
ExecStart=/path/to/websocket_server/build/camera_server 9002 85 30 1
Restart=on-failure
RestartSec=10
Environment="LD_LIBRARY_PATH=/opt/MVS/lib"

[Install]
WantedBy=multi-user.target
```

启用服务：

```bash
sudo systemctl daemon-reload
sudo systemctl enable hik-camera-server
sudo systemctl start hik-camera-server

# 查看状态
sudo systemctl status hik-camera-server

# 查看日志
sudo journalctl -u hik-camera-server -f
```

### 方法2：使用crontab

```bash
crontab -e

# 添加以下行
@reboot cd /path/to/websocket_server && ./run_server.sh
```

## 故障排除

### 问题1：找不到OpenCV

```bash
# 检查OpenCV是否安装
pkg-config --modversion opencv4
# 或
pkg-config --modversion opencv

# 如果未安装
sudo apt-get install libopencv-dev
```

### 问题2：找不到HIK SDK库

```bash
# 检查库文件
ls /opt/MVS/lib

# 设置库路径
export LD_LIBRARY_PATH=/opt/MVS/lib:$LD_LIBRARY_PATH

# 或在run_server.sh中修改路径
```

### 问题3：相机初始化失败

```bash
# 检查相机连接
lsusb  # USB相机
ifconfig  # 网络相机

# 检查权限
sudo chmod 666 /dev/video*

# 添加用户到video组
sudo usermod -a -G video $USER
```

### 问题4：编译错误

```bash
# 清理并重新编译
rm -rf build
./build.sh

# 检查CMake版本
cmake --version  # 需要 >= 3.10

# 检查g++版本
g++ --version  # 需要支持C++14
```

### 问题5：端口被占用

```bash
# 查看端口占用
sudo netstat -tulpn | grep 9002

# 或使用lsof
sudo lsof -i :9002

# 杀死占用进程
sudo kill -9 <PID>
```

### 问题6：无法连接（防火墙）

```bash
# 临时关闭防火墙测试
sudo ufw disable

# 如果可以连接，则添加规则
sudo ufw allow 9002/tcp
sudo ufw enable
```

## 性能优化

### 1. 使用Release模式编译

CMakeLists.txt中已默认使用Release模式，如需修改：

```cmake
set(CMAKE_BUILD_TYPE Release)
```

### 2. 启用编译器优化

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
```

### 3. 调整线程数

```bash
# 编译时使用所有CPU核心
make -j$(nproc)
```

### 4. 系统优化

```bash
# 提高文件描述符限制
ulimit -n 65535

# 优化网络参数
sudo sysctl -w net.core.rmem_max=26214400
sudo sysctl -w net.core.wmem_max=26214400
```

## 远程访问

### SSH隧道

```bash
# 在本地机器上
ssh -L 9002:localhost:9002 user@server_ip

# 然后在浏览器访问
ws://localhost:9002
```

### Nginx反向代理

```nginx
server {
    listen 80;
    server_name your_domain.com;

    location /camera {
        proxy_pass http://localhost:9002;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

## 日志管理

### 重定向输出到日志文件

```bash
./run_server.sh > server.log 2>&1 &

# 实时查看日志
tail -f server.log
```

### 使用logrotate管理日志

创建 `/etc/logrotate.d/hik-camera-server`：

```
/path/to/websocket_server/server.log {
    daily
    rotate 7
    compress
    delaycompress
    missingok
    notifempty
}
```

## 测试和验证

### 1. 测试相机连接

```bash
cd build
./camera_server
# 观察是否有相机初始化成功的消息
```

### 2. 测试WebSocket连接

```bash
# 使用wscat工具
npm install -g wscat
wscat -c ws://localhost:9002
```

### 3. 性能测试

```bash
# 监控CPU和内存
top -p $(pgrep camera_server)

# 监控网络流量
iftop -i eth0
```

## 卸载

```bash
# 停止服务
sudo systemctl stop hik-camera-server
sudo systemctl disable hik-camera-server

# 删除服务文件
sudo rm /etc/systemd/system/hik-camera-server.service
sudo systemctl daemon-reload

# 删除项目文件
cd ..
rm -rf websocket_server

# 卸载OpenCV（可选）
sudo apt-get remove libopencv-dev
```

## 常用命令速查

```bash
# 安装依赖
./setup_dependencies.sh

# 编译
./build.sh

# 运行
./run_server.sh

# 后台运行
nohup ./run_server.sh > server.log 2>&1 &

# 查看进程
ps aux | grep camera_server

# 停止服务
pkill camera_server

# 查看日志
tail -f server.log

# 查看端口
sudo netstat -tulpn | grep 9002

# 测试连接
curl -i -N -H "Connection: Upgrade" -H "Upgrade: websocket" http://localhost:9002
```

## 技术支持

如遇到问题：
1. 检查日志输出
2. 验证依赖是否正确安装
3. 确认相机连接和权限
4. 查看防火墙设置
5. 参考故障排除章节

更多信息请参考：
- [README.md](README.md) - 完整文档
- [QUICKSTART.md](QUICKSTART.md) - 快速入门
- [CROSSHAIR_GUIDE.md](CROSSHAIR_GUIDE.md) - 准星功能
