/**
 * 准星功能示例程序
 * 演示如何使用和自定义准星功能
 */

#include "camera_websocket_server.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void example_basic_crosshair() {
    std::cout << "\n=== 示例1：基本准星使用 ===" << std::endl;
    std::cout << "启动服务器，使用默认绿色准星..." << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    
    if (server.start()) {
        std::cout << "服务器已启动，准星已启用（绿色，大小30）" << std::endl;
        std::cout << "按Enter键继续..." << std::endl;
        std::cin.get();
        server.stop();
    }
}

void example_custom_color() {
    std::cout << "\n=== 示例2：自定义准星颜色 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    
    // 设置为红色准星
    server.set_crosshair_color(255, 0, 0);
    std::cout << "准星颜色设置为红色" << std::endl;
    
    if (server.start()) {
        std::cout << "服务器已启动，观察红色准星效果" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 切换到蓝色
        server.set_crosshair_color(0, 0, 255);
        std::cout << "准星颜色切换为蓝色" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 切换到黄色
        server.set_crosshair_color(255, 255, 0);
        std::cout << "准星颜色切换为黄色" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        server.stop();
    }
}

void example_size_adjustment() {
    std::cout << "\n=== 示例3：调整准星大小 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    server.set_crosshair_color(255, 0, 0);  // 红色便于观察
    
    if (server.start()) {
        // 小准星
        server.set_crosshair_size(15);
        std::cout << "小准星（大小15）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 中等准星
        server.set_crosshair_size(30);
        std::cout << "中等准星（大小30）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 大准星
        server.set_crosshair_size(50);
        std::cout << "大准星（大小50）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        server.stop();
    }
}

void example_thickness_adjustment() {
    std::cout << "\n=== 示例4：调整线条粗细 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    server.set_crosshair_color(0, 255, 0);  // 绿色
    server.set_crosshair_size(40);           // 较大尺寸便于观察
    
    if (server.start()) {
        // 细线
        server.set_crosshair_thickness(1);
        std::cout << "细线条（粗细1）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 中等
        server.set_crosshair_thickness(2);
        std::cout << "中等线条（粗细2）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 粗线
        server.set_crosshair_thickness(4);
        std::cout << "粗线条（粗细4）" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        server.stop();
    }
}

void example_toggle_crosshair() {
    std::cout << "\n=== 示例5：动态开关准星 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    server.set_crosshair_color(255, 0, 0);
    
    if (server.start()) {
        for (int i = 0; i < 3; i++) {
            std::cout << "准星启用" << std::endl;
            server.set_crosshair_enabled(true);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            
            std::cout << "准星禁用" << std::endl;
            server.set_crosshair_enabled(false);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        
        server.stop();
    }
}

void example_preset_styles() {
    std::cout << "\n=== 示例6：预设准星样式 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    
    if (server.start()) {
        // 样式1：精确瞄准（小而精细）
        std::cout << "样式1：精确瞄准" << std::endl;
        server.set_crosshair_color(255, 0, 0);
        server.set_crosshair_size(15);
        server.set_crosshair_thickness(1);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 样式2：快速定位（大而明显）
        std::cout << "样式2：快速定位" << std::endl;
        server.set_crosshair_color(0, 255, 0);
        server.set_crosshair_size(50);
        server.set_crosshair_thickness(3);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 样式3：夜间模式（高对比度）
        std::cout << "样式3：夜间模式" << std::endl;
        server.set_crosshair_color(255, 255, 0);
        server.set_crosshair_size(35);
        server.set_crosshair_thickness(2);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // 样式4：竞技模式（醒目）
        std::cout << "样式4：竞技模式" << std::endl;
        server.set_crosshair_color(0, 255, 255);  // 青色
        server.set_crosshair_size(25);
        server.set_crosshair_thickness(2);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        server.stop();
    }
}

void example_interactive() {
    std::cout << "\n=== 示例7：交互式准星调整 ===" << std::endl;
    
    CameraWebSocketServer server(9002, 85, 30, true);
    
    if (!server.start()) {
        std::cerr << "服务器启动失败" << std::endl;
        return;
    }
    
    std::cout << "\n交互式准星控制" << std::endl;
    std::cout << "命令：" << std::endl;
    std::cout << "  c <r> <g> <b> - 设置颜色 (例如: c 255 0 0)" << std::endl;
    std::cout << "  s <size>      - 设置大小 (例如: s 40)" << std::endl;
    std::cout << "  t <thickness> - 设置粗细 (例如: t 3)" << std::endl;
    std::cout << "  e <0/1>       - 禁用/启用 (例如: e 1)" << std::endl;
    std::cout << "  q             - 退出" << std::endl;
    std::cout << std::endl;
    
    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;
        
        if (cmd == "q") {
            break;
        } else if (cmd == "c") {
            int r, g, b;
            std::cin >> r >> g >> b;
            server.set_crosshair_color(r, g, b);
        } else if (cmd == "s") {
            int size;
            std::cin >> size;
            server.set_crosshair_size(size);
        } else if (cmd == "t") {
            int thickness;
            std::cin >> thickness;
            server.set_crosshair_thickness(thickness);
        } else if (cmd == "e") {
            int enabled;
            std::cin >> enabled;
            server.set_crosshair_enabled(enabled != 0);
        } else {
            std::cout << "未知命令" << std::endl;
        }
    }
    
    server.stop();
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "     准星功能示例程序" << std::endl;
    std::cout << "========================================" << std::endl;
    
    int choice;
    std::cout << "\n选择示例：" << std::endl;
    std::cout << "1. 基本准星使用" << std::endl;
    std::cout << "2. 自定义准星颜色" << std::endl;
    std::cout << "3. 调整准星大小" << std::endl;
    std::cout << "4. 调整线条粗细" << std::endl;
    std::cout << "5. 动态开关准星" << std::endl;
    std::cout << "6. 预设准星样式" << std::endl;
    std::cout << "7. 交互式调整" << std::endl;
    std::cout << "\n请输入选择 (1-7): ";
    std::cin >> choice;
    std::cin.ignore();  // 清除输入缓冲
    
    switch (choice) {
        case 1:
            example_basic_crosshair();
            break;
        case 2:
            example_custom_color();
            break;
        case 3:
            example_size_adjustment();
            break;
        case 4:
            example_thickness_adjustment();
            break;
        case 5:
            example_toggle_crosshair();
            break;
        case 6:
            example_preset_styles();
            break;
        case 7:
            example_interactive();
            break;
        default:
            std::cout << "无效的选择" << std::endl;
            break;
    }
    
    std::cout << "\n示例运行完成" << std::endl;
    return 0;
}
