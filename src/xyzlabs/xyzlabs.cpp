
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

#include "xyzlabs.hpp"
#include "constants.hpp"
#include "utils.hpp"


static void glfw_error_callback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
    throw std::runtime_error(description);
}

void XYZLabs::init() {
    glfwSetErrorCallback(glfw_error_callback);

    utils::handle_event("GLFW initialization", glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1000, 1000, constants::TITLE, nullptr, nullptr);
    utils::handle_event("Window creation", window != nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwMaximizeWindow(window);

    IMGUI_CHECKVERSION();
    
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
}

int XYZLabs::mainloop() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            spdlog::info("ESC key pressed, exiting");
            glfwSetWindowShouldClose(window, true);
        } else {
            glfwGetFramebufferSize(window, &width, &height);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            ImGui::Render();
    
            glViewport(0, 0, width, height);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
    
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
            glfwSwapBuffers(window);    
        }
    }
    return 0;
}

void XYZLabs::exit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

XYZLabs& XYZLabs::instance() {
    static XYZLabs labs;
    return labs;
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

WidgetManager &XYZLabs::get_widget_manager() {
    return widgetManager;
}

TaskManager &XYZLabs::get_task_manager() {
    return taskManager;
}
