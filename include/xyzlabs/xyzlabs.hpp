
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <concepts>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/widgetmanager.hpp"


class GLFWWindow;

template<typename T>
concept DerivedFromWidget = std::derived_from<T, Widget>;

class DefaultIntroWidget: public Widget {
public:
    DefaultIntroWidget() = default;
};

class XYZLabs {
    std::string title_;
	GLFWwindow *window_ = nullptr;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    RandomGenerator randomGenerator_;
    int32_t width_ = 1000;
    int32_t height_ = 1000;

    void init_();
    void exit_();
    void mainloop_();
    XYZLabs() {
        set_initial_widget<DefaultIntroWidget>();
    };
public:
    template<DerivedFromWidget T>
    void set_initial_widget() {
        widgetManager_.add_intro_widget<T>();
    };
    void init(const std::string &title);
    static XYZLabs& instance();
    TaskManager &task_manager();
    WidgetManager &widget_manager();
    RandomGenerator& random_generator();
    void close();
    int exec();
    inline int32_t width() {
        return width_;
    }
    inline int32_t height() {
        return height_;
    }
};

#endif
