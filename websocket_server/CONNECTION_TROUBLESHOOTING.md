# WebSocket连接故障排除

## 🔴 错误：426 Upgrade required

### 错误信息
```
[error] Handshake ended with HTTP error: 426
[fail] WebSocket Connection ... / 426 websocketpp:28 Upgrade required
```

### 原因
客户端使用HTTP协议而不是WebSocket协议连接服务器。

---

## ✅ 解决方案

### 1. 检查连接地址格式

**❌ 错误的地址：**
```
http://192.168.1.100:9002
https://192.168.1.100:9002
192.168.1.100:9002
```

**✅ 正确的地址：**
```
ws://192.168.1.100:9002
```

### 2. 在手机浏览器中输入

#### 方法1：直接在地址栏输入（推荐）

1. 打开手机浏览器（Chrome/Edge/Safari）
2. 访问：`http://服务器IP:9002/web_client.html`
   - 例如：`http://192.168.1.100:9002/web_client.html`
3. **注意**：这里用http访问HTML文件，但HTML内部会用ws://连接WebSocket

#### 方法2：使用本地HTML文件

1. 将 `web_client.html` 或 `mobile_client.html` 发送到手机
2. 用浏览器打开文件
3. 在输入框填写：`ws://服务器IP:9002`
4. 点击"连接"

### 3. 确保格式正确

在客户端输入框中，地址必须以 `ws://` 开头：

```javascript
// 正确
ws://192.168.1.100:9002
ws://192.168.129.49:9002

// 错误
http://192.168.1.100:9002
192.168.1.100:9002
```

---

## 🌐 提供HTTP服务（推荐方案）

为了让手机可以直接通过浏览器访问，建议添加HTTP服务器来提供HTML文件。

### 方案1：使用Python简单HTTP服务器

在 `websocket_server` 目录下运行：

**Python 3:**
```bash
python -m http.server 8080
```

**Python 2:**
```bash
python -m SimpleHTTPServer 8080
```

然后手机访问：
```
http://服务器IP:8080/web_client.html
```

### 方案2：使用Node.js http-server

```bash
# 安装
npm install -g http-server

# 运行
http-server -p 8080
```

### 方案3：集成到C++服务器（最佳）

修改服务器代码，添加HTTP服务来提供HTML文件。

---

## 🔧 快速修复脚本

### Windows - serve_web.bat

创建文件 `serve_web.bat`:
```batch
@echo off
echo Starting HTTP server for web client...
echo.
echo Access from phone:
echo http://%COMPUTERNAME%:8080/web_client.html
echo or
echo http://YOUR_IP:8080/mobile_client.html
echo.
python -m http.server 8080
pause
```

### Ubuntu - serve_web.sh

创建文件 `serve_web.sh`:
```bash
#!/bin/bash
echo "Starting HTTP server for web client..."
echo ""
IP=$(hostname -I | awk '{print $1}')
echo "Access from phone:"
echo "http://$IP:8080/web_client.html"
echo "or"
echo "http://$IP:8080/mobile_client.html"
echo ""
python3 -m http.server 8080
```

---

## 📱 完整使用流程

### 步骤1：启动WebSocket服务器

```bash
# Windows
run_server.bat

# Ubuntu
./run_server.sh
```

服务器会显示：
```
========================================
  HIK Camera WebSocket Streaming Server
========================================
Port: 9002
...
```

### 步骤2：启动HTTP服务器（提供网页）

**新开一个终端窗口：**

```bash
# Windows
cd websocket_server
python -m http.server 8080

# Ubuntu
cd websocket_server
python3 -m http.server 8080
```

### 步骤3：手机访问

1. 确保手机和服务器在同一网络（或服务器连接手机热点）
2. 在手机浏览器输入：`http://服务器IP:8080/mobile_client.html`
3. 页面会自动连接到 `ws://服务器IP:9002`

---

## 🐛 其他常见错误

### 错误1：连接超时
```
[fail] The opening handshake timed out
```

**原因：**
- 防火墙阻止
- 服务器未运行
- IP地址错误

**解决：**
```bash
# 检查服务器是否运行
netstat -ano | findstr 9002  # Windows
lsof -i :9002                # Linux

# 检查防火墙
# Windows
netsh advfirewall firewall add rule name="WebSocket" dir=in action=allow protocol=TCP localport=9002

# Ubuntu
sudo ufw allow 9002/tcp
```

### 错误2：连接被拒绝
```
WebSocket connection to 'ws://...' failed: Connection refused
```

**原因：**
- 服务器未启动
- 端口号错误

**解决：**
- 确认服务器正在运行
- 检查端口号是否为9002

### 错误3：无法解析主机
```
Failed to resolve host
```

**原因：**
- IP地址错误
- 网络未连接

**解决：**
- 使用 `ipconfig`（Windows）或 `ifconfig`（Linux）查看正确的IP
- 确保手机和服务器在同一网络

---

## 📊 连接检查清单

- [ ] 服务器已启动（`run_server.bat/sh`）
- [ ] 防火墙已允许端口9002
- [ ] 手机和服务器在同一网络
- [ ] 使用正确的IP地址
- [ ] 地址格式为 `ws://IP:9002`（不是http://）
- [ ] 浏览器支持WebSocket（Chrome/Edge/Safari）

---

## 🎯 推荐配置

### 最简单的方式

1. **服务器连接手机热点**
2. **查看服务器IP**（通常是 192.168.43.x）
3. **手机浏览器访问**：`http://服务器IP:8080/mobile_client.html`
4. **页面自动连接WebSocket**

### 需要的命令

```bash
# 终端1：启动WebSocket服务器
./run_server.sh

# 终端2：启动HTTP服务器
python3 -m http.server 8080

# 手机浏览器访问
http://192.168.43.xxx:8080/mobile_client.html
```

---

## 💡 提示

1. **不要在浏览器地址栏直接输入 `ws://`**
   - 浏览器会尝试用HTTP访问
   - 应该访问HTML文件，让JavaScript代码建立WebSocket连接

2. **使用mobile_client.html**
   - 针对手机优化
   - 支持触摸操作
   - 响应式设计

3. **检查浏览器控制台**
   - 打开开发者工具（F12）
   - 查看Console标签
   - 会显示详细的连接错误信息

---

## 📞 快速测试

### 测试WebSocket服务器是否运行

```bash
# 使用wscat（需要安装Node.js）
npm install -g wscat
wscat -c ws://localhost:9002

# 或使用curl
curl -i -N -H "Connection: Upgrade" -H "Upgrade: websocket" http://localhost:9002
```

### 测试网络连通性

```bash
# 从手机ping服务器
ping 服务器IP

# 从服务器ping手机
ping 手机IP
```

---

**记住：访问HTML文件用 `http://`，但WebSocket连接用 `ws://`！**
