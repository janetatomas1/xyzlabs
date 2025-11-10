
#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <algorithm>

#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/globals.hpp"

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

namespace xyzlabs {

void WindowManager::init_main_window(std::unique_ptr<Window> window) {
    auto mainWindow = std::move(window);
    mainWindow->init();
    windows_.push_back(std::move(mainWindow));
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
    std::this_thread::sleep_for(std::chrono::milliseconds(renderTimeout_));
}

void WindowManager::destroy() {
    ImGui::DestroyContext();
    glfwTerminate();
}

#else

#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>

void WindowManager::init() {
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        spdlog::error("SDL init failed");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    init_main_window();
}

void WindowManager::update() {
    flush_closed_windows();
    update_windows();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if(e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            Window *window = (Window*) SDL_GetPointerProperty(
                SDL_GetWindowProperties(
                    SDL_GetWindowFromID(e.window.windowID)
                ), "WINDOW", NULL
            );
            window->close();
        }

        if(e.type == SDL_EVENT_KEY_DOWN) {
            Window *window = (Window*) SDL_GetPointerProperty(
                SDL_GetWindowProperties(
                    SDL_GetWindowFromID(e.window.windowID)
                ), "WINDOW", NULL
            );
        }
    }
}

void WindowManager::destroy() {
    ImGui::DestroyContext();
    SDL_Quit();
}

#endif

Window* WindowManager::add_window(std::unique_ptr<Window> window) {
    auto id = window->id();
    auto ptr = window.get();
    event_manager().add_action(std::move([window = std::move(window), this] () mutable {
        window->init();
        windows_.push_back(std::move(window));
    }));

    return ptr;
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
    if(currentWindowIDx_ >= 0 && windows_.size() > currentWindowIDx_) {
        return windows_[currentWindowIDx_].get();
    }

    return nullptr;
}

}
