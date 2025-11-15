# Bayer格式支持说明

## 📷 什么是Bayer格式

Bayer格式是数字相机传感器的原始输出格式，每个像素只记录一种颜色（红、绿或蓝）。需要通过**去马赛克（Demosaicing）**算法转换为RGB/BGR彩色图像。

### Bayer格式类型

| 格式 | 说明 | OpenCV转换代码 |
|------|------|----------------|
| Bayer RG | 红-绿排列 | `COLOR_BayerRG2BGR` |
| Bayer GR | 绿-红排列 | `COLOR_BayerGR2BGR` |
| Bayer BG | 蓝-绿排列 | `COLOR_BayerBG2BGR` |
| Bayer GB | 绿-蓝排列 | `COLOR_BayerGB2BGR` |

## 🔧 本项目的实现

### 相机输出格式
- **格式**: Bayer RG 8
- **位深**: 8位
- **通道数**: 1（单通道）
- **数据大小**: `width × height × 1` 字节

### 转换流程

```cpp
// 1. 分配单通道缓冲区
unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight];

// 2. 获取Bayer格式图像
camera.Photograph_background(pData, 1000);
camera.Photograph_join();

// 3. 创建单通道Mat
cv::Mat bayer_image(camera.nHeight, camera.nWidth, CV_8UC1, pData);

// 4. Bayer RG转BGR
cv::Mat bgr_image;
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR);

// 5. 后续处理（准星、JPEG编码等）
```

## 🎯 关键修改点

### 修改前（错误）
```cpp
// 错误：假设是RGB三通道
unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight * 3];
cv::Mat raw_image(camera.nHeight, camera.nWidth, CV_8UC3, pData);
cv::cvtColor(raw_image, bgr_image, cv::COLOR_RGB2BGR);
```

**问题**：
- ❌ 缓冲区大小错误（3倍）
- ❌ Mat类型错误（CV_8UC3应为CV_8UC1）
- ❌ 颜色转换错误（RGB2BGR应为BayerRG2BGR）
- ❌ 导致图像数据损坏、噪点

### 修改后（正确）
```cpp
// 正确：Bayer单通道
unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight];
cv::Mat bayer_image(camera.nHeight, camera.nWidth, CV_8UC1, pData);
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR);
```

**优点**：
- ✅ 缓冲区大小正确
- ✅ Mat类型正确
- ✅ 使用正确的Bayer转换
- ✅ 图像清晰无噪点

## 📊 性能影响

### 内存使用
- **修改前**: `width × height × 3` 字节
- **修改后**: `width × height × 1` 字节
- **节省**: 66%内存

### 处理速度
- Bayer转BGR使用硬件加速（如果可用）
- 转换速度快，延迟低

### 图像质量
- 使用OpenCV的高质量去马赛克算法
- 支持多种插值方法（默认使用最佳质量）

## 🔍 如何确定相机格式

### 方法1：查看相机文档
查看HIK相机的技术规格，找到"像素格式"或"Pixel Format"。

### 方法2：使用HIK SDK工具
```cpp
// 获取像素格式
MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
nRet = MV_CC_GetImageBuffer(handle, &stFrameInfo, 1000);

// 检查 stFrameInfo.enPixelType
// PixelType_Gvsp_BayerRG8 = Bayer RG 8
```

### 方法3：测试不同转换
如果不确定具体格式，可以尝试所有Bayer类型：

```cpp
// 测试所有Bayer格式
cv::Mat bgr_rg, bgr_gr, bgr_bg, bgr_gb;
cv::cvtColor(bayer_image, bgr_rg, cv::COLOR_BayerRG2BGR);
cv::cvtColor(bayer_image, bgr_gr, cv::COLOR_BayerGR2BGR);
cv::cvtColor(bayer_image, bgr_bg, cv::COLOR_BayerBG2BGR);
cv::cvtColor(bayer_image, bgr_gb, cv::COLOR_BayerGB2BGR);

// 保存并比较
cv::imwrite("test_rg.jpg", bgr_rg);
cv::imwrite("test_gr.jpg", bgr_gr);
cv::imwrite("test_bg.jpg", bgr_bg);
cv::imwrite("test_gb.jpg", bgr_gb);

// 颜色正确的就是正确格式
```

## 🛠️ 其他Bayer格式支持

如果您的相机使用其他Bayer格式，修改转换代码：

### Bayer GR
```cpp
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerGR2BGR);
```

### Bayer BG
```cpp
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerBG2BGR);
```

### Bayer GB
```cpp
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerGB2BGR);
```

## 📝 常见问题

### Q: 图像颜色偏红/绿/蓝？
A: 可能使用了错误的Bayer格式。尝试其他格式（RG/GR/BG/GB）。

### Q: 图像有马赛克效果？
A: 可能没有进行Bayer转换，或使用了错误的Mat类型。

### Q: 图像有彩色噪点？
A: 检查缓冲区大小是否正确（单通道应为 `width × height`）。

### Q: 如何提高转换质量？
A: OpenCV的Bayer转换已经使用了高质量算法，通常不需要额外优化。

### Q: 支持Bayer 10/12/16位吗？
A: 当前实现支持Bayer 8位。如需支持更高位深：

```cpp
// Bayer 10/12/16位需要使用CV_16UC1
cv::Mat bayer_image(camera.nHeight, camera.nWidth, CV_16UC1, pData);
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR);
```

## 🎨 高级选项

### 使用VNG算法（更高质量）
```cpp
// 需要OpenCV 4.x+
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR_VNG);
```

### 使用EA算法（边缘感知）
```cpp
cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR_EA);
```

### 转换为RGB而非BGR
```cpp
cv::cvtColor(bayer_image, rgb_image, cv::COLOR_BayerRG2RGB);
```

## ✅ 验证转换正确性

### 检查清单
- [ ] 缓冲区大小 = `width × height × 1`
- [ ] Mat类型 = `CV_8UC1`
- [ ] 转换代码 = `COLOR_BayerRG2BGR`
- [ ] 图像无噪点
- [ ] 颜色正常
- [ ] FPS正常

### 测试方法
1. 编译并运行服务器
2. 查看日志无错误
3. 手机连接查看图像
4. 图像应清晰、颜色正常、无噪点

## 📚 参考资料

- [OpenCV Bayer转换文档](https://docs.opencv.org/4.x/de/d25/imgproc_color_conversions.html)
- [Bayer滤波器原理](https://en.wikipedia.org/wiki/Bayer_filter)
- [HIK相机SDK文档](https://www.hikrobotics.com/)

---

**现在图像应该正常显示了！** 🎉
