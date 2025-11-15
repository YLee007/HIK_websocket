# HIK相机WebSocket实时视频流传输系统 - 项目总结

## 📋 项目概述

本项目实现了一个完整的海康威视工业相机实时视频流传输系统，通过WebSocket协议将相机采集的图像实时传输到手机、平板或电脑浏览器上。

## 🏗️ 系统架构

```
┌─────────────┐     多线程采集      ┌──────────────┐     JPEG编码
│  海康相机    │ ──────────────────> │ HIK_VideoCapture │ ──────────>
└─────────────┘                     └──────────────┘

                                           │
                                           ▼
                                    ┌──────────────┐
                                    │ JPEG压缩     │
                                    └──────────────┘
                                           │
                                           ▼
                                    ┌──────────────┐     WebSocket
                                    │ WS服务器     │ ──────────────>
                                    └──────────────┘
                                           │
                        ┌──────────────────┼──────────────────┐
                        ▼                  ▼                  ▼
                   ┌────────┐        ┌────────┐        ┌────────┐
                   │ 手机1  │        │ 手机2  │        │ 电脑   │
                   └────────┘        └────────┘        └────────┘
```

## 📁 项目文件结构

```
websocket_server/
├── camera_websocket_server.hpp    # WebSocket服务器类头文件
├── camera_websocket_server.cpp    # WebSocket服务器实现
├── main.cpp                        # 主程序入口
├── example_usage.cpp               # 使用示例代码
├── CMakeLists.txt                  # CMake构建配置
├── web_client.html                 # 桌面版Web客户端
├── mobile_client.html              # 移动端优化客户端
├── setup_dependencies.bat          # 依赖下载脚本（Windows）
├── build.bat                       # 编译脚本（Windows）
├── run_server.bat                  # 运行脚本（Windows）
├── README.md                       # 完整文档
├── QUICKSTART.md                   # 快速入门指南
├── PROJECT_SUMMARY.md              # 本文件
└── .gitignore                      # Git忽略配置
```

## 🔧 核心组件

### 1. CameraWebSocketServer类

**功能**：
- 管理WebSocket服务器
- 多线程相机图像采集
- JPEG编码和压缩
- 多客户端广播

**关键方法**：
```cpp
CameraWebSocketServer(int port, int jpeg_quality, int frame_rate);
bool start();                    // 启动服务器
void stop();                     // 停止服务器
bool is_running();               // 检查运行状态
int get_client_count();          // 获取客户端数量
```

### 2. Web客户端

**两个版本**：
- `web_client.html` - 桌面版，功能完整，UI美观
- `mobile_client.html` - 移动端优化，支持全屏、触摸操作

**功能特性**：
- 实时视频显示
- FPS和延迟监控
- 连接状态指示
- 可配置服务器地址
- 全屏模式
- 响应式设计

## 🚀 快速开始

### 最简单的使用方式（3步）

1. **下载依赖**
   ```bash
   setup_dependencies.bat
   ```

2. **编译项目**
   ```bash
   build.bat
   ```

3. **运行服务器**
   ```bash
   run_server.bat
   ```

### 连接客户端

1. 打开 `web_client.html` 或 `mobile_client.html`
2. 输入服务器地址（例如：`ws://192.168.1.100:9002`）
3. 点击"连接"

## 📊 技术规格

### 性能指标

| 指标 | 典型值 | 可调范围 |
|------|--------|----------|
| 帧率 | 30 fps | 1-60 fps |
| 延迟 | 50-100ms | 取决于网络 |
| JPEG质量 | 85 | 1-100 |
| 带宽占用 | 10-15 Mbps | 取决于分辨率和质量 |

### 支持的平台

**服务器端**：
- Windows 10/11
- Linux (Ubuntu, Debian, etc.)

**客户端**：
- iOS Safari
- Android Chrome
- 桌面浏览器（Chrome, Firefox, Edge, Safari）

## 🔌 依赖项

### 必需依赖

1. **OpenCV** (>= 3.0) - 图像处理
2. **WebSocket++** - WebSocket协议实现
3. **Asio** - 异步I/O
4. **HIK MVS SDK** - 海康相机SDK

### 可选依赖

- CMake (>= 3.10) - 构建工具
- Git - 下载第三方库

## 💡 使用场景

1. **远程监控** - 在手机上查看相机画面
2. **调试辅助** - 实时查看相机采集效果
3. **多人协作** - 多个客户端同时查看
4. **移动演示** - 无需专用软件即可展示

## ⚙️ 配置选项

### 服务器参数

```cpp
// 端口号
int port = 9002;

// JPEG质量（1-100）
// 70-80: 低延迟，适合移动网络
// 85-90: 平衡质量和带宽
// 90-95: 高质量，需要良好网络
int jpeg_quality = 85;

// 目标帧率
// 15-20: 省带宽
// 25-30: 流畅体验
// 30+: 高帧率（需要高性能网络）
int frame_rate = 30;
```

### 命令行使用

```bash
camera_server.exe [端口] [JPEG质量] [帧率]

# 示例
camera_server.exe 9002 85 30
camera_server.exe 8080 70 20  # 低延迟配置
camera_server.exe 9002 95 15  # 高质量配置
```

## 🔒 安全注意事项

⚠️ **重要提示**：

1. **默认无加密** - 使用ws://协议，数据未加密
2. **无身份验证** - 任何人都可以连接
3. **局域网使用** - 建议仅在可信网络使用
4. **防火墙配置** - 确保端口安全配置

### 安全增强建议

- 升级到WSS（WebSocket Secure）
- 添加用户认证机制
- 使用VPN访问
- 限制IP白名单

## 🐛 故障排除

### 常见问题

**Q: 编译失败，找不到websocketpp**
```
A: 运行 setup_dependencies.bat 下载依赖库
```

**Q: 相机初始化失败**
```
A: 检查相机连接，确认HIK SDK已安装
```

**Q: 手机无法连接**
```
A: 1. 确认手机和电脑在同一WiFi
   2. 使用电脑IP而非localhost
   3. 检查防火墙设置
```

**Q: 视频卡顿**
```
A: 1. 降低帧率到15-20fps
   2. 降低JPEG质量到70-80
   3. 使用有线网络
```

## 📈 性能优化建议

### 降低延迟
- JPEG质量：70-80
- 帧率：15-20 fps
- 使用有线网络
- 减少客户端数量

### 提高画质
- JPEG质量：90-95
- 确保网络带宽充足（20+ Mbps）
- 使用千兆以太网

### 节省带宽
- 降低分辨率（相机端设置）
- JPEG质量：60-70
- 帧率：10-15 fps

## 🔮 未来扩展

### 计划功能
- [ ] WSS加密传输
- [ ] 用户认证系统
- [ ] H.264/H.265视频编码
- [ ] 录像功能
- [ ] 多相机支持
- [ ] 云台控制
- [ ] 移动侦测
- [ ] AI分析集成

### 贡献方式
欢迎提交Issue和Pull Request！

## 📝 许可证

MIT License - 自由使用和修改

## 📞 技术支持

- 查看 [README.md](README.md) 获取详细文档
- 查看 [QUICKSTART.md](QUICKSTART.md) 快速入门
- 查看 [example_usage.cpp](example_usage.cpp) 代码示例

## 🎯 总结

本项目提供了一个**完整、易用、高性能**的工业相机视频流传输解决方案：

✅ **易于部署** - 3个脚本完成安装和运行  
✅ **跨平台** - 支持Windows/Linux服务器，任意设备客户端  
✅ **高性能** - 多线程架构，30fps流畅传输  
✅ **可定制** - 丰富的配置选项和示例代码  
✅ **开箱即用** - 包含完整的客户端和服务器代码  

---

**开始使用**：运行 `setup_dependencies.bat` → `build.bat` → `run_server.bat`

**需要帮助**：查看 [QUICKSTART.md](QUICKSTART.md)
