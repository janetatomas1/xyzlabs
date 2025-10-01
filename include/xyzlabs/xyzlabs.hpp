
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

class XYZLabs {
    std::string title_;
    
    TaskManager taskManager_;
    RandomGenerator randomGenerator_;
    SettingsManager settingsManager_;
    EventManager eventManager_;
    WindowManager windowManager_;

    void init_();
    void exit_();
    void mainloop_();
    XYZLabs() = default;

    int exitCode_ = 0;
    template<WidgetType W = Widget, typename... Args>
    void set_initial_widget(Args... args);
public:
    template<WidgetType W = Widget, typename... Args>
    XYZLabs& init(const std::string &title, Args... args);
    static XYZLabs& instance();
    TaskManager &task_manager();
    RandomGenerator& random_generator();
    SettingsManager& settings_manager();
    EventManager& event_manager();
    WindowManager& window_manager();

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
    windowManager_.get_main_window()->set_central_widget<W>(std::forward<Args>(args)...);
};

template<WidgetType W, typename... Args>
XYZLabs& XYZLabs::init(const std::string &title, Args... args) {
    title_ = title;
    init_();
    set_initial_widget<W>(std::forward<Args>(args)...);
    return instance();
}

#endif
