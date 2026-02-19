
#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <algorithm>

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/utils/randomgenerator.hpp"
#include "xyzlabs/assert.hpp"

namespace xyzlabs {

WindowManager::WindowManager(XYZLabs* app) : app_(app) {
    XYZ_ASSERT_MSG(app_, "WindowManager requires a valid XYZLabs app");
}

void WindowManager::init_main_window(std::unique_ptr<Window> window) {
    XYZ_ASSERT_MSG(window, "Cannot submit null window");

    auto mainWindow = std::move(window);
    mainWindow->init();
    mainWindow->set_window_manager(this);

    if(windows_.size() == 0) {
        windows_.push_back(std::move(mainWindow));
    } else {
        windows_[0] = std::move(mainWindow);
    }
}

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
}

void WindowManager::update() {
    flush_closed_windows();
    update_windows();
    glfwPollEvents();
}

void WindowManager::destroy() {
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

Window* WindowManager::get_current_window() {
    XYZ_ASSERT_MSG(
        currentWindowIDx_ < windows_.size(),
        "currentWindowIDx_ out of bounds"
    );
    if(windows_.size() > currentWindowIDx_) {
        return windows_[currentWindowIDx_].get();
    }

    return nullptr;
}

size_t WindowManager::current_window_index() const {
    return currentWindowIDx_;
}

Window* WindowManager::submit_window(std::unique_ptr<Window> window) {
    XYZ_ASSERT_MSG(window, "Cannot submit null window");

    auto ptr = window.get();
    window->set_window_manager(this);
    app()->event_manager().add_action(std::move([window = std::move(window), this] () mutable {
        window->init();
        windows_.push_back(std::move(window));
    }));

    return ptr;
}

XYZLabs* WindowManager::app() {
    return app_;
}

}
