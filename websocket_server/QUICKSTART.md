# 快速入门指南

## 5分钟快速开始

### Windows系统

#### 步骤1：安装依赖（首次使用）

1. **安装Git**（如果还没有）
   - 下载：https://git-scm.com/download/win

2. **安装CMake**
   - 下载：https://cmake.org/download/

3. **安装Visual Studio 2019或更高版本**
   - 下载社区版（免费）：https://visualstudio.microsoft.com/

4. **安装OpenCV**
   - 下载预编译版本：https://opencv.org/releases/
   - 解压到 `C:\opencv`
   - 添加环境变量：`C:\opencv\build\x64\vc15\bin`

#### 步骤2：下载WebSocket库

双击运行：
```
setup_dependencies.bat
```

这会自动下载WebSocket++和Asio库。

#### 步骤3：编译项目

双击运行：
```
build.bat
```

#### 步骤4：运行服务器

双击运行：
```
run_server.bat
```

服务器启动后会显示你的IP地址。

---

### Ubuntu/Linux系统

#### 步骤1：设置权限并安装依赖

```bash
cd websocket_server
chmod +x *.sh
./setup_dependencies.sh
```

这会自动安装所有必需的依赖。

#### 步骤2：编译项目

```bash
./build.sh
```

#### 步骤3：运行服务器

```bash
./run_server.sh
```

**详细的Ubuntu安装指南：** [UBUNTU_GUIDE.md](UBUNTU_GUIDE.md)

### 步骤5：连接手机

1. 确保手机和电脑在同一WiFi网络
2. 用浏览器打开 `web_client.html` 文件
3. 或者将 `web_client.html` 发送到手机，用手机浏览器打开
4. 修改服务器地址为你的电脑IP（例如：`ws://192.168.1.100:9002`）
5. 点击"连接"按钮

## 常见问题

### Q: 编译失败，提示找不到OpenCV
**A**: 确保已安装OpenCV并设置了环境变量。在CMakeLists.txt中可以手动指定OpenCV路径：
```cmake
set(OpenCV_DIR "C:/opencv/build")
```

### Q: 手机无法连接
**A**: 
1. 检查防火墙设置，允许端口9002
2. 确保使用电脑的局域网IP，不是localhost
3. 确认手机和电脑在同一网络

### Q: 视频很卡
**A**: 
1. 降低帧率：`camera_server.exe 9002 85 15`
2. 降低质量：`camera_server.exe 9002 70 30`
3. 使用有线网络

## 自定义参数

编辑 `run_server.bat`，修改这一行：
```batch
camera_server.exe <端口> <JPEG质量> <帧率>
```

例如：
```batch
camera_server.exe 9002 90 25
```

## 下一步

- 阅读完整的 [README.md](README.md) 了解更多功能
- 查看源代码进行自定义开发
- 添加身份验证和加密功能

## 技术支持

如遇到问题，请检查：
1. 相机是否正确连接
2. HIK SDK是否安装
3. 防火墙设置
4. 网络连接

详细的故障排除指南请参考 [README.md](README.md)。
