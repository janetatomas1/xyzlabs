
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

size_t WindowManager::nwindows() const {
    return windows_.size();
}


Window* WindowManager::get_main_window() const {
    if(windows_.empty()) {
        return nullptr;
    }

    return windows_[0].get();
}

Window* WindowManager::get_window_by_id(uint64_t id) const {
    auto it = std::find_if(
        windows_.begin(),
        windows_.end(),
        [id](const auto &win) {
            return win->id() == id;
        }
    );

    if(it == windows_.end()) {
        return nullptr;
    } else {
        return it->get();
    }
}

Window* WindowManager::get_window_by_title(const std::string &title) const {
    auto it = std::find_if(
        windows_.begin(),
        windows_.end(),
        [title](const auto &win) {
            return win->title() == title;
        }
    );

    if(it == windows_.end()) {
        return nullptr;
    } else {
        return it->get();
    }
}
