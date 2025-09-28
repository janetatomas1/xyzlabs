
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "xyzlabs/widgetmanager.hpp"

class ImGuiContext;
class GLFWwindow;

class Window {
    GLFWwindow *handle_ = nullptr;
    ImGuiContext *ctx;

    std::string title_;
    WidgetManager widgetManager_;
    int32_t width_ = 1000;
    int32_t height_ = 1000;
public:
    Window();
    ~Window();
    void update();
    bool should_close() const;
    GLFWwindow* handle();
    virtual void key_callback(int key);
    void reset_key_callback();
};

#endif
