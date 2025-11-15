#include "camera_websocket_server.hpp"
#include <iostream>
#include <chrono>

CameraWebSocketServer::CameraWebSocketServer(int port, int quality, int fps, bool crosshair)
    : jpeg_quality(quality), frame_rate(fps), running(false), 
      enable_crosshair(crosshair), crosshair_color(0, 255, 0), 
      crosshair_size(30), crosshair_thickness(2) {
    
    // 设置WebSocket服务器
    ws_server.init_asio();
    ws_server.set_reuse_addr(true);
    
    // 绑定回调函数
    ws_server.set_open_handler(std::bind(&CameraWebSocketServer::on_open, this, std::placeholders::_1));
    ws_server.set_close_handler(std::bind(&CameraWebSocketServer::on_close, this, std::placeholders::_1));
    ws_server.set_message_handler(std::bind(&CameraWebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    
    // 监听端口
    ws_server.listen(port);
    ws_server.start_accept();
    
    std::cout << "[WebSocket Server] Initialized on port " << port << std::endl;
}

CameraWebSocketServer::~CameraWebSocketServer() {
    stop();
}

void CameraWebSocketServer::on_open(connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(conn_mutex);
    connections.insert(hdl);
    std::cout << "[WebSocket] Client connected. Total clients: " << connections.size() << std::endl;
}

void CameraWebSocketServer::on_close(connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(conn_mutex);
    connections.erase(hdl);
    std::cout << "[WebSocket] Client disconnected. Total clients: " << connections.size() << std::endl;
}

void CameraWebSocketServer::on_message(connection_hdl hdl, message_ptr msg) {
    // 处理客户端消息（如控制命令）
    std::string payload = msg->get_payload();
    std::cout << "[WebSocket] Received message: " << payload << std::endl;
}

void CameraWebSocketServer::broadcast_frame(const std::vector<uchar>& jpeg_data) {
    std::lock_guard<std::mutex> lock(conn_mutex);
    
    for (auto it : connections) {
        try {
            ws_server.send(it, jpeg_data.data(), jpeg_data.size(), websocketpp::frame::opcode::binary);
        } catch (const std::exception& e) {
            std::cerr << "[WebSocket] Error sending frame: " << e.what() << std::endl;
        }
    }
}

void CameraWebSocketServer::camera_capture_loop() {
    // 初始化相机
    int ret = camera.Init_Video();
    if (ret != 0) {
        std::cerr << "[Camera] Initialization failed with code: " << ret << std::endl;
        running = false;
        return;
    }
    
    std::cout << "[Camera] Initialized successfully. Resolution: " 
              << camera.nWidth << "x" << camera.nHeight << std::endl;
    
    // 分配图像缓冲区 (Bayer RG 8是单通道格式)
    unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight];
    
    // 计算帧间隔
    auto frame_interval = std::chrono::milliseconds(1000 / frame_rate);
    
    while (running) {
        auto start_time = std::chrono::steady_clock::now();
        
        // 多线程拍摄
        camera.Photograph_background(pData, 1000);
        camera.Photograph_join();
        
        if (camera.Photograph_ret == 0) {
            // 检查数据是否为空
            bool data_valid = false;
            for (int i = 0; i < 100; i++) {
                if (pData[i] != 0) {
                    data_valid = true;
                    break;
                }
            }
            
            if (!data_valid) {
                std::cerr << "[Camera] Warning: Image data appears to be empty" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // 创建Bayer格式的Mat (单通道)
            cv::Mat bayer_image(camera.nHeight, camera.nWidth, CV_8UC1, pData);
            
            // 检查Mat是否有效
            if (bayer_image.empty()) {
                std::cerr << "[Camera] Error: Failed to create Mat from image data" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // Bayer RG 8转换为BGR
            cv::Mat bgr_image;
            try {
                // Bayer RG格式使用COLOR_BayerRG2BGR
                cv::cvtColor(bayer_image, bgr_image, cv::COLOR_BayerRG2BGR);
            } catch (const cv::Exception& e) {
                std::cerr << "[Camera] Bayer conversion error: " << e.what() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // 绘制准星
            if (enable_crosshair) {
                draw_crosshair(bgr_image);
            }
            
            // 编码为JPEG
            std::vector<uchar> jpeg_buffer;
            std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, jpeg_quality};
            bool encode_success = cv::imencode(".jpg", bgr_image, jpeg_buffer, params);
            
            if (!encode_success || jpeg_buffer.empty()) {
                std::cerr << "[Camera] JPEG encoding failed" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // 广播到所有客户端
            if (!connections.empty()) {
                broadcast_frame(jpeg_buffer);
            }
            
            // 控制帧率
            auto end_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            auto sleep_time = frame_interval - elapsed;
            
            if (sleep_time.count() > 0) {
                std::this_thread::sleep_for(sleep_time);
            }
        } else {
            std::cerr << "[Camera] Capture failed with code: " << camera.Photograph_ret << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    // 清理
    delete[] pData;
    camera.Close_Video();
    std::cout << "[Camera] Stopped" << std::endl;
}

bool CameraWebSocketServer::start() {
    if (running) {
        std::cerr << "[Server] Already running" << std::endl;
        return false;
    }
    
    running = true;
    
    // 启动WebSocket服务器线程
    server_thread = std::thread([this]() {
        try {
            std::cout << "[WebSocket Server] Starting..." << std::endl;
            ws_server.run();
        } catch (const std::exception& e) {
            std::cerr << "[WebSocket Server] Error: " << e.what() << std::endl;
        }
    });
    
    // 启动相机采集线程
    camera_thread = std::thread(&CameraWebSocketServer::camera_capture_loop, this);
    
    std::cout << "[Server] Started successfully" << std::endl;
    return true;
}

void CameraWebSocketServer::stop() {
    if (!running) {
        return;
    }
    
    std::cout << "[Server] Stopping..." << std::endl;
    running = false;
    
    // 停止WebSocket服务器
    ws_server.stop();
    
    // 等待线程结束
    if (camera_thread.joinable()) {
        camera_thread.join();
    }
    
    if (server_thread.joinable()) {
        server_thread.join();
    }
    
    std::cout << "[Server] Stopped" << std::endl;
}

int CameraWebSocketServer::get_client_count() {
    std::lock_guard<std::mutex> lock(conn_mutex);
    return connections.size();
}

// 绘制准星
void CameraWebSocketServer::draw_crosshair(cv::Mat& image) {
    int center_x = image.cols / 2;
    int center_y = image.rows / 2;
    
    // 绘制十字准星
    // 水平线
    cv::line(image, 
             cv::Point(center_x - crosshair_size, center_y), 
             cv::Point(center_x + crosshair_size, center_y), 
             crosshair_color, crosshair_thickness, cv::LINE_AA);
    
    // 垂直线
    cv::line(image, 
             cv::Point(center_x, center_y - crosshair_size), 
             cv::Point(center_x, center_y + crosshair_size), 
             crosshair_color, crosshair_thickness, cv::LINE_AA);
    
    // 绘制中心圆点
    cv::circle(image, cv::Point(center_x, center_y), 3, crosshair_color, -1, cv::LINE_AA);
    
    // 可选：绘制外圈
    cv::circle(image, cv::Point(center_x, center_y), crosshair_size / 2, 
               crosshair_color, crosshair_thickness, cv::LINE_AA);
}

// 准星控制方法
void CameraWebSocketServer::set_crosshair_enabled(bool enabled) {
    enable_crosshair = enabled;
    std::cout << "[Crosshair] " << (enabled ? "Enabled" : "Disabled") << std::endl;
}

void CameraWebSocketServer::set_crosshair_color(int r, int g, int b) {
    crosshair_color = cv::Scalar(b, g, r);  // OpenCV uses BGR
    std::cout << "[Crosshair] Color set to RGB(" << r << ", " << g << ", " << b << ")" << std::endl;
}

void CameraWebSocketServer::set_crosshair_size(int size) {
    crosshair_size = size;
    std::cout << "[Crosshair] Size set to " << size << std::endl;
}

void CameraWebSocketServer::set_crosshair_thickness(int thickness) {
    crosshair_thickness = thickness;
    std::cout << "[Crosshair] Thickness set to " << thickness << std::endl;
}
