
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <imgui.h>
#include <memory>

#include <boost/uuid.hpp>

#include "taskmanager.hpp"
#include "widgetmanager.hpp"

namespace uuid = boost::uuids;

class GLFWwindow;


class XYZLabs {
    GLFWwindow *window_;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    uuid::random_generator idGenerator_;

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
    uuid::random_generator& id_generator();
};

#endif
