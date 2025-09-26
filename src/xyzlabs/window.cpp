
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "xyzlabs/window.hpp"

Window::Window() {
    handle_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
    if(handle_) {
        spdlog::error("GLFW window creation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW window creation SUCCESS!");
    }
    glfwMaximizeWindow(windowManager_.window_);
}

Window::~Window() {
    glfwDestroyWindow(handle_);
    glfwTerminate();
}
