
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <concepts>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "xyzlabs/idgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/widgetmanager.hpp"


class GLFWWindow;

template<typename T>
concept DerivedFromWidget = std::derived_from<T, Widget>;

class XYZLabs {
    std::string title_;
	GLFWwindow *window_ = nullptr;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    IDGenerator idGenerator_;
    int32_t width_ = 1000;
    int32_t height_ = 1000;

    void init_();
    void exit_();
    void mainloop_();
    XYZLabs() = default;
public:
    template<DerivedFromWidget T>
    void init(const std::string &title);
    static XYZLabs& instance();
    TaskManager &task_manager();
    WidgetManager &widget_manager();
    IDGenerator& id_generator();
    void close();
    int exec();
    inline int32_t width() {
        return width_;
    }
    inline int32_t height() {
        return height_;
    }
};

template<DerivedFromWidget T>
void XYZLabs::init(const std::string &title) {
    widgetManager_.add_widget<T>();
    title_ = title;
}

#endif
