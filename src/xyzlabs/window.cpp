
#include <GL/glew.h>

#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "xyzlabs/window.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/globals.hpp"

#ifdef USE_GLFW

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

namespace xyzlabs {

void Window::init() {
    bool maximize = false;
    if(height_ == 0 || width_ == 0) {
        width_ = 5000;
        height_ = 5000;
        maximize = true;
    }

    handle_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
    if(!handle_) {
        spdlog::error("GLFW window creation failed!");
        glfwTerminate();
    } else {
        spdlog::info("GLFW window creation SUCCESS!");
    }

    if(maximize) {
        glfwMaximizeWindow(handle_);
    }
    glfwMakeContextCurrent(handle_);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        spdlog::error("GLEW initialisation failed!");
    } else {
        spdlog::info("GLEW initialisation SUCCESS!");
    }

    IMGUI_CHECKVERSION();
    ctx = ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;

    make_context_current();

    ImGui_ImplGlfw_InitForOpenGL(handle_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glfwSetWindowUserPointer(handle_, this);

    spdlog::info("Opened new window. Title: {}, id: {}", title_, id_);
}

Window::~Window() {
    make_context_current();

    centralWidget_->destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctx);

    glfwDestroyWindow(handle_);
    spdlog::info("Closed window. Title: {}, id: {}", title_, id_);
}

void Window::update() {
    make_context_current();
    glfwSwapInterval(0);

    glfwGetWindowSize(handle_, &width_, &height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();    
    
    ImGui::NewFrame();

    ImVec2 size = {static_cast<float>(width_), static_cast<float>(height_)};
    ImVec2 pos = {0.0f, 0.0f};

    centralWidget_->display(size, pos);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
    glfwSwapBuffers(handle_);

    key_callback();

    if(glfwWindowShouldClose(handle_)) {
        open_ = false;
    }
}

GLFWwindow* Window::handle() {
    return handle_;
}

void Window::make_context_current() {
    glfwMakeContextCurrent(handle_);
    ImGui::SetCurrentContext(ctx);
}

#else

#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>

void Window::init() {
    handle_ = SDL_CreateWindow(
        title_.c_str(), width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    glContext = SDL_GL_CreateContext(handle_);
    SDL_GL_MakeCurrent(handle_, glContext);

    IMGUI_CHECKVERSION();
    ImGui::GetIO().IniFilename = nullptr;
    ctx = ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;

    make_context_current();

    SDL_ShowWindow(handle_);
    SDL_MaximizeWindow(handle_);

    SDL_SetPointerProperty(SDL_GetWindowProperties(handle_), "WINDOW", this);
    ImGui_ImplSDL3_InitForOpenGL(handle_, glContext);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        spdlog::error("GLEW initialisation failed!");
    } else {
        spdlog::info("GLEW initialisation SUCCESS!");
    }
    spdlog::info("Opened new window. Title: {}, id: {}", title_, id_);
}

Window::~Window() {
    make_context_current();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(handle_);
    
    spdlog::info("Closed window. Title: {}, id: {}", title_, id_);
}

void Window::update() {
    make_context_current();

    SDL_GetWindowSize(handle_, &width_, &height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImVec2 size = {static_cast<float>(width_), static_cast<float>(height_)};
    ImVec2 pos = {0.0f, 0.0f};
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    if(ImGui::Begin("##window", nullptr,  WINDOW_FLAGS)) {
        centralWidget_->show(size, pos);
    }
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(handle_);

    key_callback();
}

WindowHandle Window::handle() {
    return handle_;
}

void Window::make_context_current() {
    SDL_GL_MakeCurrent(handle_, glContext);
    ImGui::SetCurrentContext(ctx);
}

#endif


Window::Window(const std::string &title, int32_t width, int32_t height):
    title_(title),
    id_(random_generator()()),
    centralWidget_(std::move(std::make_unique<Widget>())),
    width_(width),
    height_(height)
{}

void Window::close() {
    event_manager().add_action([this]() {
        open_ = false;
    });
}

Widget* Window::submit_widget(std::unique_ptr<Widget> widget) {
    auto ptr = widget.get();
    auto action = [this, widget = std::move(widget)]() mutable {
        make_context_current();
        if(centralWidget_ != nullptr) {
            centralWidget_->destroy();
        }

        widget->set_window(this);
        centralWidget_ = std::move(widget);
        centralWidget_->init();
    };
    event_manager().add_action(std::move(action));
    return ptr;
}

Widget* Window::set_central_widget(std::unique_ptr<Widget> widget) {
    return submit_widget(std::move(widget));
}

bool Window::is_open() const {
    return open_;
}

void Window::key_callback() {
    if(ImGui::IsKeyDown(ImGuiKey_Escape)) {
        close();
    }
}

void Window::set_color(const std::array<float, 4> &color) {
    event_manager().add_action([this, color] () {
        make_context_current();
        if(ctx->ColorStack.Size > 0) {
            ImGui::PopStyleColor();
        }
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255));
    });
}

auto flip_image(uint8_t *data, size_t width, size_t height) {
    int row_bytes = width * 4;
    std::vector<unsigned char> temp(row_bytes);

    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row_top = data + y * row_bytes;
        unsigned char* row_bottom = data + (height - 1 - y) * row_bytes;
        std::copy(row_top, row_top + row_bytes, temp.begin());
        std::copy(row_bottom, row_bottom + row_bytes, row_top);
        std::copy(temp.begin(), temp.end(), row_bottom);
    }
}

bool Window::export_png(const std::string &filename) {
    std::vector<unsigned char> pixels(width_ * height_ * 4);

    make_context_current();
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    flip_image(&pixels[0], width_, height_);

    return stbi_write_png(filename.c_str(), width_, height_, 4, pixels.data(), width_ * 4) != 0;
}

ImGuiStyle& Window::style() {
    return ImGui::GetStyle();
}

}
