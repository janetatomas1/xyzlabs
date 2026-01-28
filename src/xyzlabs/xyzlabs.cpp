
#include <imgui.h>

#include <GL/glew.h>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <cstdlib>
#include <fmt/format.h>
#include <thread>

#include "xyzlabs/utils/utils.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

XYZLabs::XYZLabs(): windowManager_(this), settingsManager_() {}

void XYZLabs::init_() {
    windowManager_.init();
    taskManager_.run();
    settingsManager_.init();

    IMGUI_CHECKVERSION();
}

void XYZLabs::mainloop_() {
    while(true) {
        if(windowManager_.nwindows() == 0) {
            return;
        }

        eventManager_.dispatch();
        windowManager_.update();
        frame_++;
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

int XYZLabs::return_code() {
    return exitCode_;
}

uint64_t XYZLabs::frame() {
    return frame_;
}

size_t XYZLabs::timeout() {
    return renderTimeout_;
}

void XYZLabs::set_timeout(size_t timeout) {
    renderTimeout_ = timeout;
}

RandomGenerator &XYZLabs::random_generator() {
    static RandomGenerator generator;
    return generator;
}

}
