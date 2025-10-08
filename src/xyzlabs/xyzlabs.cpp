
#include <imgui.h>
#include <imgui_impl_glfw.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stdexcept>
#include <cstdlib>
#include <fmt/format.h>
#include <thread>

#include "xyzlabs/utils.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"


void XYZLabs::init_() {
    windowManager_.init();

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    create_app_directory();
    taskManager_.run();
    settingsManager_.init();
}

void XYZLabs::mainloop_() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(windowManager_.nwindows() == 0) {
            return;
        }

        eventManager_.dispatch();
        windowManager_.update();
    }
}

void XYZLabs::exit_() {
    taskManager_.stop();
    spdlog::info("Closed {}", title_);
}

int XYZLabs::exec() {
    try {
        mainloop_();
        exit_();
    } catch(std::exception &e) {
        exitCode_ = 1;
    }
    return exitCode_;
}

XYZLabs& XYZLabs::instance() {
    static XYZLabs wave;
    return wave;
}

TaskManager &XYZLabs::task_manager() {
    return taskManager_;
}

RandomGenerator &XYZLabs::random_generator() {
    return randomGenerator_;
}

SettingsManager &XYZLabs::settings_manager() {
    return settingsManager_;
}

EventManager& XYZLabs::event_manager() {
    return eventManager_;
}

WindowManager &XYZLabs::window_manager() {
    return windowManager_;
}

void XYZLabs::close() {
}

const std::string& XYZLabs::title() {
    return title_;
}

std::filesystem::path XYZLabs::app_directory() {
    auto titleStandardized = utils::standardize(title());
    
    #ifdef __unix__
        return std::filesystem::path(std::getenv("HOME")) / fmt::format(".{}", titleStandardized);
    #else
        return (std::filesystem::path(std::getenv("USERPROFILE")) / fmt::format(".{}", titleStandardized)).string();
    #endif
}

std::filesystem::path XYZLabs::create_app_directory() {
    auto appDirectory = app_directory();

    if(!std::filesystem::is_directory(appDirectory)) {
        std::filesystem::create_directory(appDirectory);
    }

    return appDirectory;
}
