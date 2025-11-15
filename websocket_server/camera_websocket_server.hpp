#ifndef CAMERA_WEBSOCKET_SERVER_HPP
#define CAMERA_WEBSOCKET_SERVER_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <opencv2/opencv.hpp>
#include <set>
#include <mutex>
#include <thread>
#include <atomic>
#include "../HIK_TEF_Driver_module/HIK_TEF_Driver.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;
typedef websocketpp::connection_hdl connection_hdl;

class CameraWebSocketServer {
private:
    server ws_server;
    std::set<connection_hdl, std::owner_less<connection_hdl>> connections;
    std::mutex conn_mutex;
    std::thread server_thread;
    std::thread camera_thread;
    std::atomic<bool> running;
    
    HIK_VideoCapture camera;
    int jpeg_quality;
    int frame_rate;
    bool enable_crosshair;
    cv::Scalar crosshair_color;
    int crosshair_size;
    int crosshair_thickness;
    
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, message_ptr msg);
    void camera_capture_loop();
    void broadcast_frame(const std::vector<uchar>& jpeg_data);
    void draw_crosshair(cv::Mat& image);
    
public:
    CameraWebSocketServer(int port = 9002, int quality = 85, int fps = 30, bool crosshair = true);
    ~CameraWebSocketServer();
    
    bool start();
    void stop();
    bool is_running() const { return running; }
    int get_client_count();
    
    // 准星控制
    void set_crosshair_enabled(bool enabled);
    void set_crosshair_color(int r, int g, int b);
    void set_crosshair_size(int size);
    void set_crosshair_thickness(int thickness);
};

#endif
