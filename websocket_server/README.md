# HIK相机WebSocket实时视频流传输系统

这是一个基于WebSocket的海康威视工业相机实时视频流传输系统，可以将相机采集的图像实时传输到手机、平板或电脑的浏览器上。

## 功能特性

- ✅ 多线程相机图像采集
- ✅ JPEG压缩减少带宽占用
- ✅ WebSocket实时传输
- ✅ 支持多客户端同时连接
- ✅ 可调节JPEG质量和帧率
- ✅ 响应式Web界面，支持手机和平板
- ✅ 实时显示FPS、延迟等统计信息
- ✅ **图像中心准星标记**（可自定义颜色、大小、粗细）

## 系统架构

```
海康相机 → HIK_VideoCapture (多线程采集) → JPEG编码 → WebSocket服务器 → 手机/浏览器
```

## 依赖项

### 必需依赖

1. **OpenCV** (>= 3.0)
   - 用于图像处理和JPEG编码
   - Windows: 从官网下载预编译版本
   - Linux: `sudo apt-get install libopencv-dev`

2. **WebSocket++** (仅头文件库)
   - 下载地址: https://github.com/zaphoyd/websocketpp
   - 解压到 `websocket_server/third_party/websocketpp`

3. **Asio** (仅头文件库)
   - 下载地址: https://github.com/chriskohlhoff/asio
   - 解压到 `websocket_server/third_party/asio`

4. **HIK MVS SDK**
   - 海康威视机器视觉工业相机SDK
   - 已包含在 `HIK_TEF_Driver_module/include` 和 `lib` 目录

## 安装步骤

### Windows系统

#### 1. 下载依赖库

```bash
cd websocket_server
setup_dependencies.bat
```

#### 2. 编译项目

```bash
build.bat
```

#### 3. 运行服务器

```bash
run_server.bat
```

### Ubuntu/Linux系统

#### 1. 设置脚本权限并安装依赖

```bash
cd websocket_server
chmod +x *.sh
./setup_dependencies.sh
```

#### 2. 编译项目

```bash
./build.sh
```

#### 3. 运行服务器

```bash
./run_server.sh
```

**详细的Ubuntu安装指南请参考：[UBUNTU_GUIDE.md](UBUNTU_GUIDE.md)**

### 3. 运行服务器

```bash
# 使用默认参数（端口9002，JPEG质量85，帧率30fps）
./camera_server

# 自定义参数
./camera_server <端口> <JPEG质量> <帧率>
# 例如：
./camera_server 9002 90 25
```

### 4. 连接客户端

#### 方法1：使用提供的Web客户端

1. 用浏览器打开 `web_client.html`
2. 修改服务器地址为你的电脑IP（例如：`ws://192.168.1.100:9002`）
3. 点击"连接"按钮

#### 方法2：手机访问

1. 确保手机和电脑在同一局域网
2. 查看电脑IP地址：
   - Windows: `ipconfig`
   - Linux: `ifconfig` 或 `ip addr`
3. 在手机浏览器中打开 `http://<电脑IP>:端口/web_client.html`
   - 或者直接打开 `web_client.html` 文件并修改连接地址

## 使用说明

### 服务器参数

```cpp
CameraWebSocketServer(int port, int jpeg_quality, int frame_rate, bool enable_crosshair)
```

- **port**: WebSocket服务器端口（默认9002）
- **jpeg_quality**: JPEG压缩质量，1-100（默认85）
  - 质量越高，图像越清晰，但带宽占用越大
  - 建议范围：70-95
- **frame_rate**: 目标帧率（默认30fps）
  - 实际帧率取决于相机性能和网络带宽
- **enable_crosshair**: 启用准星标记（默认true）
  - 在图像中心绘制十字准星
  - 可运行时调整颜色、大小、粗细

### 准星功能

在图像中心绘制瞄准准星，适用于瞄准辅助、对齐参考等场景。

```cpp
// 启用/禁用准星
server.set_crosshair_enabled(true);

// 设置准星颜色（RGB）
server.set_crosshair_color(255, 0, 0);  // 红色

// 设置准星大小（像素）
server.set_crosshair_size(40);

// 设置线条粗细
server.set_crosshair_thickness(3);
```

详细说明请参考 [CROSSHAIR_GUIDE.md](CROSSHAIR_GUIDE.md)

### 网络配置

#### 局域网访问
- 确保防火墙允许指定端口的入站连接
- Windows防火墙设置：
  ```
  控制面板 → Windows Defender防火墙 → 高级设置 → 入站规则 → 新建规则
  选择"端口" → TCP → 特定本地端口(9002) → 允许连接
  ```

#### 外网访问（需要路由器端口转发）
1. 登录路由器管理界面
2. 找到"端口转发"或"虚拟服务器"设置
3. 添加规则：外部端口9002 → 内部IP:9002

## 性能优化建议

### 降低延迟
1. 减少JPEG质量（70-80）
2. 降低分辨率（在相机端设置）
3. 使用有线网络代替WiFi
4. 降低帧率到15-20fps

### 提高画质
1. 提高JPEG质量（90-95）
2. 确保网络带宽充足
3. 减少同时连接的客户端数量

### 带宽估算
```
带宽(Mbps) ≈ 分辨率(像素) × 帧率(fps) × 压缩比 × 8 / 1,000,000

示例（1920x1080, 30fps, JPEG质量85）:
约 10-15 Mbps
```

## 故障排除

### 问题1：编译错误 - 找不到websocketpp
**解决方案**: 确保已下载WebSocket++和Asio到 `third_party` 目录

### 问题2：相机初始化失败
**解决方案**: 
- 检查相机是否正确连接
- 确认HIK SDK已正确安装
- 检查相机是否被其他程序占用

### 问题3：手机无法连接
**解决方案**:
- 确认手机和电脑在同一网络
- 检查防火墙设置
- 使用电脑IP而非localhost
- 确认服务器已启动

### 问题4：视频卡顿
**解决方案**:
- 降低帧率
- 降低JPEG质量
- 检查网络连接质量
- 减少客户端连接数

## 技术细节

### 多线程架构
- **主线程**: 运行WebSocket服务器
- **相机采集线程**: 持续采集图像并编码
- **每个客户端**: 独立的WebSocket连接

### 数据流
1. 相机采集原始图像（RGB格式）
2. 转换为BGR格式（OpenCV标准）
3. JPEG压缩
4. 通过WebSocket发送二进制数据
5. 浏览器接收并显示

### 安全注意事项
- 默认配置无加密（ws://），不建议在公网使用
- 如需加密，可升级为WSS（需要SSL证书）
- 建议添加身份验证机制

## 扩展功能建议

- [ ] 添加用户认证
- [ ] 支持WSS加密传输
- [ ] 录像功能
- [ ] 多相机支持
- [ ] 云台控制
- [ ] 移动侦测
- [ ] H.264视频编码（更高效）

## 许可证

本项目基于MIT许可证开源。

## 联系方式

如有问题或建议，请提交Issue。
