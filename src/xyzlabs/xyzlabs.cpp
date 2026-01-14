
#include <imgui.h>

#include <GL/glew.h>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <cstdlib>
#include <fmt/format.h>
#include <thread>

#include "xyzlabs/utils.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

XYZLabs::XYZLabs(): windowManager_(this), settingsManager_(this) {}

void XYZLabs::init_() {
    windowManager_.init();
    taskManager_.run();
    settingsManager_.init();

    IMGUI_CHECKVERSION();

    create_app_directory();
}

void XYZLabs::mainloop_() {
    while(true) {
        if(windowManager_.nwindows() == 0) {
            return;
        }

        eventManager_.dispatch();
        windowManager_.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(renderTimeout_));
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

TaskManager &XYZLabs::task_manager() {
    return taskManager_;
}

SettingsManager &XYZLabs::settings_manager() {
    return settingsManager_;
}

EventManager &XYZLabs::event_manager() {
    return eventManager_;
}

WindowManager &XYZLabs::window_manager() {
    return windowManager_;
}

ObjectRegistry &XYZLabs::object_registry() {
    return objectRegistry_;
}

void XYZLabs::close() {
}

const std::string& XYZLabs::title() {
    return title_;
}

void XYZLabs::set_title(const std::string& title) {
    title_ = title;
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

RandomGenerator &XYZLabs::random_generator() {
    static RandomGenerator generator;
    return generator;
}

}
