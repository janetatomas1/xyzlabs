
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "xyzlabs/shape.hpp"
#include "xyzlabs/idgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/resultmanager.hpp"


class GLFWWindow;

class XYZLabs {
    std::string title_;
	GLFWwindow *window_ = nullptr;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    IDGenerator idGenerator_;
    ResultManager resultManager_;
    int32_t width_ = 1000;
    int32_t height_ = 1000;

    void init_();
    void exit_();
    void mainloop_();
    XYZLabs() = default;
public:
    void init(const std::string &title);
    static XYZLabs& instance();
    TaskManager &task_manager();
    WidgetManager &widget_manager();
    IDGenerator& id_generator();
    ResultManager &result_manager();
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
