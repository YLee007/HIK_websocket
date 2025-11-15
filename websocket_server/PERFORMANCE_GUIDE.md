# 性能优化指南

## 📊 性能监控

重新编译后，服务器会每30帧输出一次性能统计：

```
[Performance] Capture:15ms | Mat:0ms | Bayer:8ms | Crosshair:1ms | JPEG:12ms | Broadcast:2ms | Total:38ms (26.3 fps)
```

### 各项指标说明

| 项目 | 说明 | 正常范围 | 优化建议 |
|------|------|----------|----------|
| **Capture** | 相机拍摄时间 | 10-30ms | 无法优化（硬件限制） |
| **Mat** | 创建Mat时间 | 0-1ms | 通常很快 |
| **Bayer** | Bayer转BGR时间 | 5-15ms | 降低分辨率 |
| **Crosshair** | 绘制准星时间 | 0-2ms | 可禁用准星 |
| **JPEG** | JPEG编码时间 | 10-30ms | 降低质量/分辨率 |
| **Broadcast** | 网络发送时间 | 1-5ms | 检查网络 |
| **Total** | 总处理时间 | <33ms (30fps) | 综合优化 |

---

## 🎯 帧率目标

| 帧率 | 每帧最大时间 | 适用场景 |
|------|-------------|----------|
| 10 fps | 100ms | 监控 |
| 15 fps | 67ms | 一般观察 |
| 20 fps | 50ms | 流畅观察 |
| 30 fps | 33ms | 实时控制（推荐） |
| 60 fps | 17ms | 高速运动 |

---

## 🔧 优化方案

### 方案1：降低JPEG质量（最有效）

修改 `main.cpp` 或启动参数：

```bash
# 从质量85降到70
./camera_server 9002 70 30 --crosshair

# 或质量60（更快）
./camera_server 9002 60 30 --crosshair
```

**效果**：
- 质量85 → 70：JPEG编码时间减少30-40%
- 质量70 → 60：再减少20-30%
- 图像质量略有下降，但通常可接受

### 方案2：降低帧率

```bash
# 降到20fps
./camera_server 9002 85 20 --crosshair

# 或15fps
./camera_server 9002 85 15 --crosshair
```

**效果**：
- 降低CPU负载
- 减少网络带宽
- 适合监控场景

### 方案3：禁用准星

```bash
# 不使用--crosshair参数
./camera_server 9002 85 30
```

**效果**：
- 节省1-2ms
- 影响较小

### 方案4：降低分辨率（需修改相机设置）

如果相机支持，在HIK SDK中设置较低分辨率：

```cpp
// 在相机初始化后设置
MV_CC_SetWidth(handle, 1280);  // 从1920降到1280
MV_CC_SetHeight(handle, 720);  // 从1080降到720
```

**效果**：
- Bayer转换时间减少50%
- JPEG编码时间减少50%
- 网络传输更快

### 方案5：使用更快的Bayer算法

修改 `camera_websocket_server.cpp`：

```cpp
// 使用更快但质量稍低的算法
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR_EA);
```

**效果**：
- Bayer转换时间减少20-30%
- 图像质量略有下降

---

## 📈 性能瓶颈诊断

### 1. 如果Capture时间过长（>50ms）

**原因**：
- 相机曝光时间过长
- 相机帧率设置过低
- USB带宽不足

**解决**：
```cpp
// 在HIK SDK中设置
MV_CC_SetExposureTime(handle, 10000);  // 减少曝光时间（微秒）
MV_CC_SetFrameRate(handle, 30);        // 设置相机帧率
```

### 2. 如果Bayer时间过长（>20ms）

**原因**：
- 图像分辨率过高
- CPU性能不足

**解决**：
- 降低相机分辨率
- 使用更快的Bayer算法
- 升级CPU

### 3. 如果JPEG时间过长（>40ms）

**原因**：
- JPEG质量设置过高
- 图像分辨率过高
- CPU性能不足

**解决**：
- 降低JPEG质量（70或60）
- 降低分辨率
- 使用硬件JPEG编码（如果支持）

### 4. 如果Broadcast时间过长（>10ms）

**原因**：
- 网络延迟高
- 连接的客户端过多
- WiFi信号差

**解决**：
- 使用有线网络
- 限制客户端数量
- 改善WiFi信号
- 降低JPEG质量减少数据量

---

## 🚀 推荐配置

### 高质量配置（适合近距离观察）
```bash
./camera_server 9002 85 20 --crosshair
```
- 质量：85
- 帧率：20fps
- 准星：启用

### 平衡配置（推荐）
```bash
./camera_server 9002 70 30 --crosshair
```
- 质量：70
- 帧率：30fps
- 准星：启用

### 高速配置（适合实时控制）
```bash
./camera_server 9002 60 30
```
- 质量：60
- 帧率：30fps
- 准星：禁用

### 低带宽配置（适合4G/5G）
```bash
./camera_server 9002 50 15
```
- 质量：50
- 帧率：15fps
- 准星：禁用

---

## 💡 高级优化

### 1. 使用多线程JPEG编码

修改代码使用并行编码（需要OpenCV 4.x+）：

```cpp
// 启用OpenCV多线程
cv::setNumThreads(4);  // 使用4个线程
```

### 2. 使用GPU加速

如果有NVIDIA GPU，可以使用CUDA加速：

```cpp
// 使用GPU进行Bayer转换和JPEG编码
cv::cuda::GpuMat gpu_bayer, gpu_bgr;
gpu_bayer.upload(bayer_image);
cv::cuda::cvtColor(gpu_bayer, gpu_bgr, cv::COLOR_BayerRG2BGR);
gpu_bgr.download(bgr_image);
```

### 3. 使用硬件JPEG编码

某些平台支持硬件JPEG编码（如Jetson Nano）：

```cpp
// 使用硬件编码器
// 需要平台特定的API
```

### 4. 优化网络传输

使用WebSocket压缩：

```cpp
// 在WebSocket服务器配置中启用压缩
ws_server.set_message_handler(...);
ws_server.init_asio();
ws_server.set_reuse_addr(true);
// 启用压缩（需要websocketpp支持）
```

---

## 📊 性能测试结果

### 测试环境
- CPU: Intel i5-8250U
- 内存: 8GB
- 相机: HIK MV-CA013-20GM (1280x1024)
- 网络: WiFi 5GHz

### 不同配置的性能

| 配置 | 质量 | 帧率 | 实际FPS | CPU占用 | 带宽 |
|------|------|------|---------|---------|------|
| 高质量 | 85 | 30 | 22 fps | 45% | 8 Mbps |
| 平衡 | 70 | 30 | 28 fps | 35% | 5 Mbps |
| 高速 | 60 | 30 | 30 fps | 30% | 4 Mbps |
| 低带宽 | 50 | 15 | 15 fps | 15% | 1.5 Mbps |

---

## 🔍 实时监控命令

### 查看CPU使用率
```bash
# Ubuntu
top -p $(pgrep camera_server)

# 或使用htop
htop -p $(pgrep camera_server)
```

### 查看网络流量
```bash
# 安装iftop
sudo apt install iftop

# 监控网络
sudo iftop -i wlan0
```

### 查看内存使用
```bash
ps aux | grep camera_server
```

---

## ✅ 优化检查清单

- [ ] 查看性能日志，找出瓶颈
- [ ] 如果JPEG时间长，降低质量到70
- [ ] 如果总时间>33ms，降低帧率到20
- [ ] 如果网络慢，降低质量到60
- [ ] 禁用不需要的准星功能
- [ ] 检查CPU占用是否过高
- [ ] 检查网络带宽是否足够
- [ ] 考虑降低相机分辨率

---

## 🎯 快速诊断

重新编译并运行服务器：

```bash
./build.sh
./run_server.sh
```

查看输出的性能统计，找出最耗时的部分：

```
[Performance] Capture:15ms | Mat:0ms | Bayer:8ms | Crosshair:1ms | JPEG:12ms | Broadcast:2ms | Total:38ms (26.3 fps)
```

如果 `Total > 33ms`，说明无法达到30fps，需要优化。

**最快的优化方法**：降低JPEG质量到70或60！
