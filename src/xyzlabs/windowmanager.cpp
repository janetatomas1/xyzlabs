
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <imgui_impl_glfw.h>
#include <algorithm>

#include "xyzlabs/windowmanager.hpp"


void WindowManager::init() {
    if (!glfwInit()) {
        spdlog::error("GLFW initialisation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW initialisation SUCCESS!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    windows_.push_back(std::make_unique<Window>());
    windows_.push_back(std::make_unique<Window>());
}

void WindowManager::update() {
    glfwPollEvents();

    std::erase_if(windows_, [](const auto& window) {
        return window->should_close();
    });

    for(auto &w: windows_) {
        w->update();
    }
}

void WindowManager::destroy() {
    ImGui::DestroyContext();
    glfwTerminate();
}

size_t WindowManager::nwindows() {
    return windows_.size();
}
