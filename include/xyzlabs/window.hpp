
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "xyzlabs/widgetmanager.hpp"

class ImGuiContext;

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
};

#endif
