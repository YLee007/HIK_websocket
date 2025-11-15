#include "../HIK_TEF_Driver_module/HIK_TEF_Driver.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "HIK Camera Test Program" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // 初始化相机
    HIK_VideoCapture camera;
    std::cout << "[1/5] Initializing camera..." << std::endl;
    int ret = camera.Init_Video();
    
    if (ret != 0) {
        std::cerr << "✗ Camera initialization failed with code: " << ret << std::endl;
        std::cerr << std::endl;
        std::cerr << "Possible reasons:" << std::endl;
        std::cerr << "  1. Camera not connected" << std::endl;
        std::cerr << "  2. HIK SDK not installed" << std::endl;
        std::cerr << "  3. Camera already in use by another program" << std::endl;
        std::cerr << "  4. Insufficient permissions" << std::endl;
        return 1;
    }
    
    std::cout << "✓ Camera initialized successfully" << std::endl;
    std::cout << "  Resolution: " << camera.nWidth << "x" << camera.nHeight << std::endl;
    std::cout << std::endl;
    
    // 分配缓冲区
    std::cout << "[2/5] Allocating image buffer..." << std::endl;
    unsigned char* pData = new unsigned char[camera.nWidth * camera.nHeight * 3];
    std::cout << "✓ Buffer allocated: " << (camera.nWidth * camera.nHeight * 3) << " bytes" << std::endl;
    std::cout << std::endl;
    
    // 测试拍摄
    std::cout << "[3/5] Testing image capture..." << std::endl;
    camera.Photograph_background(pData, 1000);
    camera.Photograph_join();
    
    if (camera.Photograph_ret != 0) {
        std::cerr << "✗ Image capture failed with code: " << camera.Photograph_ret << std::endl;
        delete[] pData;
        return 1;
    }
    
    std::cout << "✓ Image captured successfully" << std::endl;
    std::cout << std::endl;
    
    // 检查数据
    std::cout << "[4/5] Checking image data..." << std::endl;
    
    // 检查是否全为0
    bool all_zero = true;
    int non_zero_count = 0;
    for (int i = 0; i < camera.nWidth * camera.nHeight * 3; i++) {
        if (pData[i] != 0) {
            all_zero = false;
            non_zero_count++;
        }
    }
    
    if (all_zero) {
        std::cerr << "✗ Warning: Image data is all zeros!" << std::endl;
        std::cerr << "  Camera may not be capturing properly" << std::endl;
    } else {
        std::cout << "✓ Image data valid" << std::endl;
        std::cout << "  Non-zero pixels: " << non_zero_count << " / " << (camera.nWidth * camera.nHeight * 3) << std::endl;
        
        // 显示前10个字节
        std::cout << "  First 10 bytes: ";
        for (int i = 0; i < 10; i++) {
            std::cout << (int)pData[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // 转换为OpenCV Mat并保存
    std::cout << "[5/5] Converting to OpenCV Mat and saving..." << std::endl;
    
    try {
        cv::Mat raw_image(camera.nHeight, camera.nWidth, CV_8UC3, pData);
        
        if (raw_image.empty()) {
            std::cerr << "✗ Failed to create Mat" << std::endl;
            delete[] pData;
            return 1;
        }
        
        std::cout << "✓ Mat created successfully" << std::endl;
        std::cout << "  Size: " << raw_image.cols << "x" << raw_image.rows << std::endl;
        std::cout << "  Channels: " << raw_image.channels() << std::endl;
        std::cout << "  Type: " << raw_image.type() << std::endl;
        
        // 尝试RGB到BGR转换
        cv::Mat bgr_image;
        cv::cvtColor(raw_image, bgr_image, cv::COLOR_RGB2BGR);
        
        // 保存图像
        bool saved = cv::imwrite("test_image_rgb2bgr.jpg", bgr_image);
        if (saved) {
            std::cout << "✓ Image saved as: test_image_rgb2bgr.jpg" << std::endl;
        }
        
        // 也尝试不转换直接保存
        saved = cv::imwrite("test_image_raw.jpg", raw_image);
        if (saved) {
            std::cout << "✓ Raw image saved as: test_image_raw.jpg" << std::endl;
        }
        
        // 尝试BGR到RGB转换（如果相机输出是BGR）
        cv::Mat rgb_image;
        cv::cvtColor(raw_image, rgb_image, cv::COLOR_BGR2RGB);
        saved = cv::imwrite("test_image_bgr2rgb.jpg", rgb_image);
        if (saved) {
            std::cout << "✓ BGR->RGB image saved as: test_image_bgr2rgb.jpg" << std::endl;
        }
        
    } catch (const cv::Exception& e) {
        std::cerr << "✗ OpenCV error: " << e.what() << std::endl;
        delete[] pData;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test completed successfully!" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Please check the generated images:" << std::endl;
    std::cout << "  - test_image_rgb2bgr.jpg (RGB->BGR conversion)" << std::endl;
    std::cout << "  - test_image_raw.jpg (no conversion)" << std::endl;
    std::cout << "  - test_image_bgr2rgb.jpg (BGR->RGB conversion)" << std::endl;
    std::cout << std::endl;
    std::cout << "Compare them to determine the correct color format" << std::endl;
    
    delete[] pData;
    return 0;
}
