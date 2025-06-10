
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


void XYZLabs::init() {
    if (!glfwInit()) {
        std::cout << "GLFW initialisation failed!\n";
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    uint32_t width = 1000;
    uint32_t height = 1000;

    window_ = glfwCreateWindow(width, height, "My Window", NULL, NULL);
    glfwMaximizeWindow(window_);
    if (!window_) {
        std::cout << "GLFW creation failed!\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "glew initialisation failed!\n";
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    t0.draw_triangle();
    t0.init();
    backend.create_framebuffer(width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void XYZLabs::mainloop() {
    uint32_t width = 1000;
    uint32_t height = 1000;

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if(glfwGetKey(window_, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();    
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();
        ImGui::Begin("My Scene");

        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        backend.rescale_framebuffer(window_width, window_height);
        glViewport(0, 0, window_width, window_height);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        ImGui::GetWindowDrawList()->AddImage(
            backend.texture_id, 
            ImVec2(pos.x, pos.y), 
            ImVec2(pos.x + window_width, pos.y + window_height), 
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );

        ImGui::End();
        ImGui::Render();

        backend.bind_framebuffer();
        t0.render();
        backend.unbind_framebuffer();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	

        glfwSwapBuffers(window_);
    }
}

void XYZLabs::exit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteFramebuffers(1, &backend.FBO);
    glDeleteTextures(1, &backend.texture_id);
    glDeleteRenderbuffers(1, &backend.RBO);

    glfwDestroyWindow(window_);
    glfwTerminate();
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
