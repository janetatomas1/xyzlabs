
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <concepts>
#include <filesystem>

#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/windowmanager.hpp"


class GLFWWindow;

template<typename T>
concept DerivedFromWidget = std::derived_from<T, Widget>;

class DefaultIntroWidget: public Widget {
public:
    DefaultIntroWidget() = default;
};

class XYZLabs {
    std::string title_;
    
    TaskManager taskManager_;
    RandomGenerator randomGenerator_;
    SettingsManager settingsManager_;
    EventManager eventManager_;
    WindowManager windowManager_;

    int32_t width_ = 1000;
    int32_t height_ = 1000;

    void init_();
    void exit_();
    void mainloop_();
    XYZLabs() = default;

    int exitCode_ = 0;
public:
    template<DerivedFromWidget T>
    XYZLabs& set_initial_widget() {
        return instance();
    };
    XYZLabs& init(const std::string &title);
    static XYZLabs& instance();
    TaskManager &task_manager();
    RandomGenerator& random_generator();
    SettingsManager& settings_manager();
    EventManager& event_manager();

    void close();
    int exec();
    inline int32_t width() {
        return width_;
    }
    inline int32_t height() {
        return height_;
    }
    const std::string& title();
    int return_code() {
        return exitCode_;
    }
    std::filesystem::path app_directory();
    std::filesystem::path create_app_directory();
};

#endif
