
#pragma once

#include <concepts>
#include <filesystem>

#include <imgui.h>

#include "randomgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

class RandomGenerator;

class XYZLabs {
    size_t renderTimeout_ = 30;
    std::string title_;
    TaskManager taskManager_;
    SettingsManager settingsManager_;
    EventManager eventManager_;
    WindowManager windowManager_;
    ObjectRegistry objectRegistry_;

    void init_();
    void exit_();
    void mainloop_();

    int exitCode_ = 0;
public:
    XYZLabs();
    template<WidgetType W = Widget, typename... Args>
    void set_initial_widget(Args... args);
    template<WidgetType W = Widget, typename... Args>
    XYZLabs& init(const std::string &title, Args... args);
    template<WindowType Win, WidgetType W = Widget, typename... Args>
    XYZLabs& init(const std::string &title, Args... args);
    static XYZLabs& instance();
    TaskManager &task_manager();
    SettingsManager &settings_manager();
    EventManager &event_manager();
    WindowManager &window_manager();
    ObjectRegistry &object_registry();
    static RandomGenerator& random_generator();

    void close();
    int exec();
    const std::string& title();
    int return_code() {
        return exitCode_;
    }
    std::filesystem::path app_directory();
    std::filesystem::path create_app_directory();
};

template<WidgetType W, typename... Args>
void XYZLabs::set_initial_widget(Args... args) {
    windowManager_
    .get_main_window()
    ->set_central_widget<W>(
        std::forward<Args>(args)...
    );
};

template<WidgetType W, typename... Args>
XYZLabs& XYZLabs::init(const std::string &title, Args... args) {
    title_ = title;
    init_();
    windowManager_.init_main_window(std::make_unique<Window>(title));
    set_initial_widget<W>(std::forward<Args>(args)...);
    return instance();
}

template<WindowType Win, WidgetType W, typename... Args>
XYZLabs& XYZLabs::init(const std::string &title, Args... args) {
    title_ = title;
    init_();
    windowManager_.init_main_window(std::make_unique<Win>(title));
    set_initial_widget<W>(std::forward<Args>(args)...);
    return instance();
}

}
