
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <implot.h>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stdexcept>

#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/utils.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"


static void glfw_error_callback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
    throw std::runtime_error(description);
}

void XYZLabs::init() {
    glfwSetErrorCallback(glfw_error_callback);

    utils::handle_event("GLFW initialization", glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window_ = glfwCreateWindow(1000, 1000, constants::TITLE.c_str(), nullptr, nullptr);
    utils::handle_event("Window creation", window_ != nullptr);

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    glfwMaximizeWindow(window_);

    IMGUI_CHECKVERSION();
    
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    auto initialWidgetID = widgetManager_.add_widget<IntroWidget>();
    widgetManager_.flush_new_widgets();
    widgetManager_.disable_widget_closing(initialWidgetID);
    taskManager_.run();
}

int XYZLabs::mainloop() {
    ImVec2 size = {};
    ImVec2 pos = {0, 0};
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            spdlog::info("ESC key pressed, exiting");
            glfwSetWindowShouldClose(window_, true);
        } else {
            glfwGetFramebufferSize(window_, &width_, &height_);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            size = {static_cast<float>(width_), static_cast<float>(height_)};

            widgetManager_.show(size, pos);

            ImGui::Render();
    
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glViewport(0, 0, width_, height_);
            glfwSwapBuffers(window_);    
        }
    }
    return 0;
}

void XYZLabs::exit() {
    taskManager_.stop();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window_);
    glfwTerminate();
}

int XYZLabs::exec() {
    try {
        init();
        mainloop();
    } catch(const std::exception &e) {
        spdlog::error(e.what());
        exit();
        return -1;
    }

    exit();

    spdlog::info("XYZLabs closing");
    return 0;
}

XYZLabs& XYZLabs::instance() {
    static XYZLabs wave;
    return wave;
}

TaskManager &XYZLabs::task_manager() {
    return taskManager_;
}

WidgetManager &XYZLabs::widget_manager() {
    return widgetManager_;
}

IDGenerator &XYZLabs::id_generator() {
    return idGenerator_;
}
