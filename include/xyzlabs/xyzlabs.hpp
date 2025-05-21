
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <imgui.h>
#include <memory>

#include "xyzlabs/idgenerator.hpp"

#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/widgetmanager.hpp"

class GLFWwindow;

class XYZLabs {
    GLFWwindow *window_;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    IDGenerator idGenerator_;

    int width_ = 0, height_ = 0;
    
    void init();
    int mainloop();
    void exit();
    XYZLabs() = default;
    ~XYZLabs() = default;
public:
    XYZLabs(const XYZLabs &) = delete;
    XYZLabs & operator = (const XYZLabs &) = delete;

    int exec();
    static XYZLabs& instance();
    TaskManager &task_manager();
    WidgetManager &widget_manager();
    IDGenerator& id_generator();
};

#endif
