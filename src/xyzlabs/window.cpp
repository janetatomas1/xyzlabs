
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "xyzlabs/window.hpp"

Window::Window() {
    handle_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
    if(!handle_) {
        spdlog::error("GLFW window creation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW window creation SUCCESS!");
    }

    glfwMaximizeWindow(handle_);
    glfwMakeContextCurrent(handle_);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        spdlog::error("GLEW initialisation failed!");
    } else {
        spdlog::info("GLEW initialisation SUCCESS!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGui_ImplGlfw_InitForOpenGL(handle_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

Window::~Window() {
    glfwMakeContextCurrent(handle_);
    ImGui::SetCurrentContext(ctx);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctx);

    glfwDestroyWindow(handle_);
}

void Window::update() {
    glfwMakeContextCurrent(handle_);

    ImGui::SetCurrentContext(ctx);
    glfwGetWindowSize(handle_, &width_, &height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::NewFrame();

    ImGui::Begin("abcd");
    
    ImGui::SetWindowSize({1000, 1000});
    ImGui::Button("abcd", {200, 100});
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
    glfwSwapBuffers(handle_);
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle_) != 0;
}
