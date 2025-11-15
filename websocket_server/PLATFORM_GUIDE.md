# 跨平台使用指南

本项目支持Windows和Ubuntu/Linux系统。根据您的操作系统选择对应的使用方法。

## 📋 平台支持

| 平台 | 支持版本 | 状态 |
|------|---------|------|
| Windows | 10/11 | ✅ 完全支持 |
| Ubuntu | 18.04/20.04/22.04 LTS | ✅ 完全支持 |
| Debian | 10/11 | ✅ 应该可用 |
| CentOS/RHEL | 7/8 | ⚠️ 未测试 |
| macOS | - | ❌ 未支持 |

## 🚀 快速开始

### Windows系统

```batch
# 1. 下载依赖
setup_dependencies.bat

# 2. 编译
build.bat

# 3. 运行
run_server.bat
```

**详细文档：** [QUICKSTART.md](QUICKSTART.md)

### Ubuntu/Linux系统

```bash
# 1. 设置权限并安装依赖
chmod +x *.sh
./setup_dependencies.sh

# 2. 编译
./build.sh

# 3. 运行
./run_server.sh
```

**详细文档：** [UBUNTU_GUIDE.md](UBUNTU_GUIDE.md)

## 📁 文件对照表

| 功能 | Windows | Ubuntu/Linux |
|------|---------|--------------|
| 依赖安装 | `setup_dependencies.bat` | `setup_dependencies.sh` |
| 编译项目 | `build.bat` | `build.sh` |
| 运行服务器 | `run_server.bat` | `run_server.sh` |
| OpenCV安装 | 手动下载 | `install_opencv.sh` |

## 🔧 依赖项对比

### Windows

| 依赖 | 安装方式 |
|------|---------|
| Git | 下载安装包 |
| CMake | 下载安装包 |
| Visual Studio | 下载安装包 |
| OpenCV | 下载预编译版本 |
| WebSocket++ | 自动下载（脚本） |
| Asio | 自动下载（脚本） |
| HIK SDK | 手动安装 |

### Ubuntu/Linux

| 依赖 | 安装方式 |
|------|---------|
| Git | `apt-get install git` |
| CMake | `apt-get install cmake` |
| G++ | `apt-get install build-essential` |
| OpenCV | `apt-get install libopencv-dev` |
| WebSocket++ | 自动下载（脚本） |
| Asio | 自动下载（脚本） |
| HIK SDK | 手动安装 |

## 💡 平台特定注意事项

### Windows

1. **路径分隔符**：使用反斜杠 `\`
2. **库文件扩展名**：`.dll`
3. **可执行文件**：`.exe`
4. **环境变量**：通过系统设置配置
5. **防火墙**：Windows Defender防火墙

**常见问题：**
- OpenCV路径配置
- Visual Studio版本兼容性
- DLL文件缺失

### Ubuntu/Linux

1. **路径分隔符**：使用斜杠 `/`
2. **库文件扩展名**：`.so`
3. **可执行文件**：无扩展名
4. **环境变量**：通过 `.bashrc` 配置
5. **防火墙**：UFW或iptables

**常见问题：**
- 库路径配置（LD_LIBRARY_PATH）
- 权限问题
- 相机设备访问权限

## 🔄 跨平台开发建议

### 代码兼容性

项目使用C++14标准，确保跨平台兼容：

```cpp
// ✅ 推荐：使用标准库
#include <filesystem>  // C++17
#include <thread>
#include <chrono>

// ❌ 避免：平台特定代码
#ifdef _WIN32
    // Windows特定代码
#else
    // Linux特定代码
#endif
```

### CMake配置

CMakeLists.txt已配置为跨平台：

```cmake
# 自动检测平台
if(WIN32)
    target_link_libraries(camera_server ws2_32)
endif()

# 使用标准路径
set(CMAKE_CXX_STANDARD 14)
```

### 脚本编写

- Windows：使用 `.bat` 批处理文件
- Linux：使用 `.sh` Shell脚本，需要执行权限

## 📊 性能对比

| 指标 | Windows | Ubuntu/Linux |
|------|---------|--------------|
| 编译速度 | 中等 | 快 |
| 运行性能 | 相当 | 相当 |
| 内存占用 | 相当 | 稍低 |
| 启动时间 | 相当 | 稍快 |

## 🛠️ 开发环境推荐

### Windows

- **IDE**：Visual Studio 2019/2022
- **编译器**：MSVC
- **调试器**：Visual Studio Debugger
- **终端**：PowerShell / CMD

### Ubuntu/Linux

- **IDE**：VS Code / CLion
- **编译器**：GCC 7+ / Clang 6+
- **调试器**：GDB
- **终端**：Bash / Zsh

## 📝 配置文件位置

### Windows

```
C:\Users\<用户名>\AppData\Local\HIK_Camera\
```

### Ubuntu/Linux

```
~/.config/hik_camera/
或
/etc/hik_camera/
```

## 🔐 权限管理

### Windows

- 需要管理员权限安装依赖
- 防火墙规则需要管理员权限
- 相机访问通常无需特殊权限

### Ubuntu/Linux

- 使用 `sudo` 安装系统依赖
- 相机访问需要加入 `video` 组
- 端口 < 1024 需要 root 权限

```bash
# 添加用户到video组
sudo usermod -a -G video $USER

# 允许非root用户绑定低端口（可选）
sudo setcap 'cap_net_bind_service=+ep' /path/to/camera_server
```

## 🌐 网络配置

### Windows防火墙

```powershell
# 允许端口9002
netsh advfirewall firewall add rule name="HIK Camera Server" dir=in action=allow protocol=TCP localport=9002
```

### Linux防火墙（UFW）

```bash
# 允许端口9002
sudo ufw allow 9002/tcp
```

### Linux防火墙（iptables）

```bash
# 允许端口9002
sudo iptables -A INPUT -p tcp --dport 9002 -j ACCEPT
```

## 🐛 平台特定问题

### Windows常见问题

1. **找不到DLL文件**
   - 解决：添加到PATH或复制到程序目录

2. **OpenCV版本不匹配**
   - 解决：使用与Visual Studio版本匹配的OpenCV

3. **端口被占用**
   - 解决：使用 `netstat -ano | findstr 9002` 查找并结束进程

### Linux常见问题

1. **找不到共享库**
   - 解决：设置 `LD_LIBRARY_PATH` 或运行 `ldconfig`

2. **相机权限不足**
   - 解决：添加用户到 `video` 组

3. **端口被占用**
   - 解决：使用 `lsof -i :9002` 查找并结束进程

## 📚 相关文档

- **通用文档**
  - [README.md](README.md) - 项目总览
  - [QUICKSTART.md](QUICKSTART.md) - 快速入门
  - [CROSSHAIR_GUIDE.md](CROSSHAIR_GUIDE.md) - 准星功能
  - [CHANGELOG.md](CHANGELOG.md) - 更新日志

- **平台特定**
  - [UBUNTU_GUIDE.md](UBUNTU_GUIDE.md) - Ubuntu详细指南
  - Windows用户请参考 [QUICKSTART.md](QUICKSTART.md)

## 🤝 贡献指南

如果您在其他平台上成功运行，欢迎贡献：

1. Fork项目
2. 添加平台支持脚本
3. 更新文档
4. 提交Pull Request

## 📞 技术支持

遇到平台相关问题：

1. 查看对应平台的详细文档
2. 检查故障排除章节
3. 提交Issue（注明操作系统和版本）

---

**选择您的平台开始使用：**
- Windows → [QUICKSTART.md](QUICKSTART.md)
- Ubuntu/Linux → [UBUNTU_GUIDE.md](UBUNTU_GUIDE.md)
