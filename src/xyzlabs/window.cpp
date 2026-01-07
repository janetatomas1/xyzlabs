
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
#include "xyzlabs/xyzlabs.hpp"

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

Window::Window(const std::string &title, int32_t width, int32_t height):
    title_(title),
    id_(XYZLabs::random_generator()()),
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

std::vector<unsigned char> Window::export_img(int x, int y, int width, int height) {
    std::vector<unsigned char> pixels(width_ * height_ * 4);

    make_context_current();
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    flip_image(&pixels[0], width, height);

    return pixels;
}

ImGuiStyle& Window::style() {
    return ImGui::GetStyle();
}

WindowManager* Window::window_manager() {
    return windowManager_;
}

void Window::set_window_manager(WindowManager *windowManager) {
    windowManager_ = windowManager;
}

}
