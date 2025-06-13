
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "xyzlabs/shape.hpp"
#include "xyzlabs/openglbackend.hpp"
#include "xyzlabs/idgenerator.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/widgetmanager.hpp"

class GLFWWindow;

class XYZLabs {
	GLFWwindow *window_;
	OpenGLBackend backend_;
    TaskManager taskManager_;
    WidgetManager widgetManager_;
    IDGenerator idGenerator_;
    Triangle t0;

    void init();
    void exit();
    void mainloop();
public:
    static XYZLabs& instance();
    TaskManager &task_manager();
    WidgetManager &widget_manager();
    IDGenerator& id_generator();
    void close();
    int exec();
};

#endif
