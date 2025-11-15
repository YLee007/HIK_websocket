#include "../HIK_Camera_C/HIK_TEF_Driver_module/HIK_TEF_Driver.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "HIK Camera FPS Test" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // 初始化相机
    HIK_VideoCapture camera;
    std::cout << "[1/3] Initializing camera..." << std::endl;
    int ret = camera.Init_Video();
    
    if (ret != 0) {
        std::cerr << "Camera initialization failed with code: " << ret << std::endl;
        return 1;
    }
    
    std::cout << "Camera initialized successfully" << std::endl;
    std::cout << "Resolution: " << camera.nWidth << "x" << camera.nHeight << std::endl;
    std::cout << std::endl;
    
    // 分配缓冲区 (Bayer RG 8是单通道)
    unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight];
    
    std::cout << "[2/3] Warming up (10 frames)..." << std::endl;
    
    // 预热：拍摄10帧
    for (int i = 0; i < 10; i++) {
        camera.Photograph_background(pData, 1000);
        camera.Photograph_join();
        if (camera.Photograph_ret != 0) {
            std::cerr << "Capture failed during warmup" << std::endl;
            delete[] pData;
            return 1;
        }
    }
    
    std::cout << "Warmup completed" << std::endl;
    std::cout << std::endl;
    
    std::cout << "[3/3] Testing FPS (100 frames)..." << std::endl;
    std::cout << std::endl;
    
    // 测试：连续拍摄100帧
    const int test_frames = 100;
    int success_count = 0;
    int fail_count = 0;
    
    auto start_time = std::chrono::steady_clock::now();
    
    for (int i = 0; i < test_frames; i++) {
        auto frame_start = std::chrono::steady_clock::now();
        
        camera.Photograph_background(pData, 1000);
        camera.Photograph_join();
        
        auto frame_end = std::chrono::steady_clock::now();
        auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
        
        if (camera.Photograph_ret == 0) {
            success_count++;
            
            // 每10帧显示一次进度
            if ((i + 1) % 10 == 0) {
                std::cout << "Frame " << (i + 1) << "/" << test_frames 
                          << " - Capture time: " << frame_time << "ms" << std::endl;
            }
        } else {
            fail_count++;
            std::cerr << "Frame " << (i + 1) << " failed with code: " 
                      << camera.Photograph_ret << std::endl;
        }
    }
    
    auto end_time = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Total frames: " << test_frames << std::endl;
    std::cout << "Success: " << success_count << std::endl;
    std::cout << "Failed: " << fail_count << std::endl;
    std::cout << "Total time: " << total_time << " ms" << std::endl;
    std::cout << std::endl;
    
    if (success_count > 0) {
        double avg_time = (double)total_time / success_count;
        double fps = 1000.0 / avg_time;
        
        std::cout << "Average time per frame: " << avg_time << " ms" << std::endl;
        std::cout << "Camera FPS: " << fps << " fps" << std::endl;
        std::cout << std::endl;
        
        // 性能评估
        std::cout << "Performance Assessment:" << std::endl;
        if (fps >= 25) {
            std::cout << "✓ Excellent - Camera can support 30fps" << std::endl;
        } else if (fps >= 15) {
            std::cout << "⚠ Good - Camera can support 15-20fps" << std::endl;
        } else if (fps >= 10) {
            std::cout << "⚠ Fair - Camera can support 10-15fps" << std::endl;
        } else {
            std::cout << "✗ Poor - Camera FPS is too low" << std::endl;
            std::cout << "  Possible reasons:" << std::endl;
            std::cout << "  - Exposure time too long" << std::endl;
            std::cout << "  - USB bandwidth insufficient" << std::endl;
            std::cout << "  - Camera settings need adjustment" << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "Recommendation:" << std::endl;
    
    if (success_count > 0) {
        double fps = 1000.0 * success_count / total_time;
        if (fps >= 25) {
            std::cout << "  Use: ./camera_server 9002 70 30 --crosshair" << std::endl;
        } else if (fps >= 15) {
            std::cout << "  Use: ./camera_server 9002 70 20 --crosshair" << std::endl;
        } else if (fps >= 10) {
            std::cout << "  Use: ./camera_server 9002 70 15 --crosshair" << std::endl;
        } else {
            std::cout << "  Use: ./camera_server 9002 60 10" << std::endl;
            std::cout << "  Consider adjusting camera exposure settings" << std::endl;
        }
    }
    
    delete[] pData;
    camera.Close_Video();
    
    return 0;
}
