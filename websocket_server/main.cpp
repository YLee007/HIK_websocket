#include "camera_websocket_server.hpp"
#include <iostream>
#include <csignal>

CameraWebSocketServer* server_ptr = nullptr;

void signal_handler(int signal) {
    std::cout << "\n[Main] Received signal " << signal << ", shutting down..." << std::endl;
    if (server_ptr) {
        server_ptr->stop();
    }
}

int main(int argc, char* argv[]) {
    // 默认参数
    int port = 9002;
    int jpeg_quality = 85;  // JPEG质量 (1-100)
    int frame_rate = 30;    // 帧率
    bool enable_crosshair = true;  // 启用准星
    
    // 解析命令行参数
    if (argc > 1) port = std::atoi(argv[1]);
    if (argc > 2) jpeg_quality = std::atoi(argv[2]);
    if (argc > 3) frame_rate = std::atoi(argv[3]);
    if (argc > 4) enable_crosshair = (std::atoi(argv[4]) != 0);
    
    std::cout << "========================================" << std::endl;
    std::cout << "  HIK Camera WebSocket Streaming Server" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "JPEG Quality: " << jpeg_quality << std::endl;
    std::cout << "Frame Rate: " << frame_rate << " fps" << std::endl;
    std::cout << "Crosshair: " << (enable_crosshair ? "Enabled" : "Disabled") << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 创建服务器
    CameraWebSocketServer server(port, jpeg_quality, frame_rate, enable_crosshair);
    server_ptr = &server;
    
    // 注册信号处理
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    // 启动服务器
    if (!server.start()) {
        std::cerr << "[Main] Failed to start server" << std::endl;
        return 1;
    }
    
    std::cout << "\n[Main] Server is running. Press Ctrl+C to stop." << std::endl;
    std::cout << "[Main] Connect from your phone browser: http://<your-ip>:" << port << std::endl;
    
    // 保持运行
    while (server.is_running()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "[Main] Server stopped" << std::endl;
    return 0;
}
