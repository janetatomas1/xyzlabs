
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>

#ifndef USE_GLFW
#include <GLFW/glfw3.h>
#endif

#include "xyzlabs/widgetmanager.hpp"

class Window {
#ifndef USE_GLFW
    GLFWwindow *window_ = nullptr;
#endif

    std::string title_;    
    WidgetManager widgetManager_;
    int32_t width_ = 1000;
    int32_t height_ = 1000;
public:
    Window();
    ~Window() = default;
};

#endif
