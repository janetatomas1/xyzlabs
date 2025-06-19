
#include "xyzlabs/xyzlabs.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stdexcept>

#include "xyzlabs/utils.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"

constexpr size_t MAIN_WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;


void XYZLabs::init() {
    if (!glfwInit()) {
        spdlog::error("GLFW initialisation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW initialisation SUCCESS!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    uint32_t width = 1000;
    uint32_t height = 1000;

    window_ = glfwCreateWindow(width, height, constants::TITLE.c_str(), NULL, NULL);
    if (!window_) {
        spdlog::error("GLFW creation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW creation SUCCESS!");
    }
    glfwMaximizeWindow(window_);

    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        spdlog::error("GLEW initialisation failed!");
        glfwDestroyWindow(window_);
        glfwTerminate();
    } else {
        spdlog::info("GLEW initialisation SUCCESS!");
    }

    t0.draw_triangle();
    t0.init();
    backend_.create_framebuffer(width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    widgetManager_.add_widget<IntroWidget>();
}

void XYZLabs::mainloop() {
    int32_t width = 0;
    int32_t height = 0;

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if(glfwGetKey(window_, GLFW_KEY_ESCAPE)) {
            spdlog::info("ESC key pressed, closing!");
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        glfwGetWindowSize(window_, &width, &height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();    
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();
        ImGui::Begin(constants::MAIN_WINDOW_ID.c_str(), nullptr, MAIN_WINDOW_FLAGS);

        widgetManager_.show({static_cast<float>(width), static_cast<float>(height)}, {0.0, 0.0});

        backend_.rescale_framebuffer(width, height);
        
        ImGui::End();
        ImGui::Render();

        backend_.bind_framebuffer();
        backend_.unbind_framebuffer();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	

        glfwSwapBuffers(window_);
    }
}

void XYZLabs::exit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteFramebuffers(1, &backend_.FBO);
    glDeleteTextures(1, &backend_.texture_id);
    glDeleteRenderbuffers(1, &backend_.RBO);

    glfwDestroyWindow(window_);
    glfwTerminate();
    spdlog::info("Closed XYZLABS");
}

int XYZLabs::exec() {
    init();
    mainloop();
    exit();
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

void XYZLabs::close() {
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}
