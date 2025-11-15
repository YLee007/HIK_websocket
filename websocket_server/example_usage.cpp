/**
 * HIK Camera WebSocket Server - 使用示例
 * 
 * 这个文件展示了如何在你的项目中使用CameraWebSocketServer类
 */

#include "camera_websocket_server.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// 示例1：基本使用
void example_basic() {
    std::cout << "=== 示例1：基本使用 ===" << std::endl;
    
    // 创建服务器（端口9002，JPEG质量85，帧率30fps）
    CameraWebSocketServer server(9002, 85, 30);
    
    // 启动服务器
    if (server.start()) {
        std::cout << "服务器已启动，按Enter键停止..." << std::endl;
        std::cin.get();
        
        // 停止服务器
        server.stop();
    } else {
        std::cerr << "服务器启动失败" << std::endl;
    }
}

// 示例2：自定义参数
void example_custom_params() {
    std::cout << "=== 示例2：自定义参数 ===" << std::endl;
    
    // 低延迟配置（降低质量和帧率）
    CameraWebSocketServer low_latency_server(9002, 70, 20);
    
    // 高质量配置（提高质量，降低帧率）
    // CameraWebSocketServer high_quality_server(9002, 95, 15);
    
    if (low_latency_server.start()) {
        std::cout << "低延迟服务器已启动" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));
        low_latency_server.stop();
    }
}

// 示例3：监控客户端连接
void example_monitor_clients() {
    std::cout << "=== 示例3：监控客户端连接 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30);
    
    if (server.start()) {
        std::cout << "服务器已启动，监控客户端连接..." << std::endl;
        
        // 每5秒检查一次客户端数量
        for (int i = 0; i < 12; i++) {  // 运行1分钟
            std::this_thread::sleep_for(std::chrono::seconds(5));
            int client_count = server.get_client_count();
            std::cout << "当前连接的客户端数量: " << client_count << std::endl;
        }
        
        server.stop();
    }
}

// 示例4：集成到现有应用
class MyApplication {
private:
    CameraWebSocketServer* ws_server;
    bool running;
    
public:
    MyApplication() : ws_server(nullptr), running(false) {}
    
    ~MyApplication() {
        stop();
    }
    
    bool start() {
        // 创建WebSocket服务器
        ws_server = new CameraWebSocketServer(9002, 85, 30);
        
        if (!ws_server->start()) {
            delete ws_server;
            ws_server = nullptr;
            return false;
        }
        
        running = true;
        std::cout << "应用程序已启动" << std::endl;
        return true;
    }
    
    void stop() {
        if (ws_server) {
            ws_server->stop();
            delete ws_server;
            ws_server = nullptr;
        }
        running = false;
        std::cout << "应用程序已停止" << std::endl;
    }
    
    void run() {
        while (running) {
            // 你的应用程序逻辑
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            // 可以在这里添加其他功能
            // 例如：处理用户输入、执行其他任务等
        }
    }
    
    int get_connected_clients() {
        return ws_server ? ws_server->get_client_count() : 0;
    }
};

void example_integration() {
    std::cout << "=== 示例4：集成到现有应用 ===" << std::endl;
    
    MyApplication app;
    
    if (app.start()) {
        std::cout << "按Enter键停止应用..." << std::endl;
        
        // 在后台线程运行应用
        std::thread app_thread([&app]() {
            app.run();
        });
        
        // 等待用户输入
        std::cin.get();
        
        // 停止应用
        app.stop();
        
        if (app_thread.joinable()) {
            app_thread.join();
        }
    }
}

// 示例5：错误处理
void example_error_handling() {
    std::cout << "=== 示例5：错误处理 ===" << std::endl;
    
    try {
        CameraWebSocketServer server(9002, 85, 30);
        
        if (!server.start()) {
            std::cerr << "错误：服务器启动失败" << std::endl;
            std::cerr << "可能的原因：" << std::endl;
            std::cerr << "1. 端口已被占用" << std::endl;
            std::cerr << "2. 相机未连接或初始化失败" << std::endl;
            std::cerr << "3. 权限不足" << std::endl;
            return;
        }
        
        std::cout << "服务器运行中..." << std::endl;
        
        // 检查服务器状态
        if (server.is_running()) {
            std::cout << "服务器状态：正常运行" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(10));
        server.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "异常：" << e.what() << std::endl;
    }
}

// 主函数 - 运行所有示例
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  HIK Camera WebSocket Server Examples" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    int choice;
    std::cout << "选择要运行的示例：" << std::endl;
    std::cout << "1. 基本使用" << std::endl;
    std::cout << "2. 自定义参数" << std::endl;
    std::cout << "3. 监控客户端连接" << std::endl;
    std::cout << "4. 集成到现有应用" << std::endl;
    std::cout << "5. 错误处理" << std::endl;
    std::cout << "请输入选择 (1-5): ";
    std::cin >> choice;
    std::cin.ignore();  // 清除输入缓冲
    
    std::cout << std::endl;
    
    switch (choice) {
        case 1:
            example_basic();
            break;
        case 2:
            example_custom_params();
            break;
        case 3:
            example_monitor_clients();
            break;
        case 4:
            example_integration();
            break;
        case 5:
            example_error_handling();
            break;
        default:
            std::cout << "无效的选择" << std::endl;
            break;
    }
    
    std::cout << std::endl;
    std::cout << "示例运行完成" << std::endl;
    
    return 0;
}
