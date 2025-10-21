
#include <GL/glew.h>

#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "xyzlabs/window.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/globals.hpp"

constexpr ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;

#ifdef USE_GLFW

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

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
    ImGui::CreateContext();
    ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGui_ImplGlfw_InitForOpenGL(handle_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glfwSetWindowUserPointer(handle_, this);

    // glfwSetKeyCallback(handle_, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
    //     Window *win = (Window*)glfwGetWindowUserPointer(handle);
    //     win->key_callback(key);
    // });
    spdlog::info("Opened new window. Title: {}, id: {}", title_, id_);
}

Window::~Window() {
    glfwMakeContextCurrent(handle_);
    ImGui::SetCurrentContext(ctx);

    centralWidget_->destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctx);

    glfwDestroyWindow(handle_);
    spdlog::info("Closed window. Title: {}, id: {}", title_, id_);
}

void Window::update() {
    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(0);

    ImGui::SetCurrentContext(ctx);
    glfwGetWindowSize(handle_, &width_, &height_);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
    glfwSwapBuffers(handle_);

    if(glfwWindowShouldClose(handle_)) {
        open_ = false;
    }
}

GLFWwindow* Window::handle() {
    return handle_;
}

void Window::key_callback(int key) {
    if(key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(handle_, 1);
    }
}

#else

#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>

void Window::close() {
    event_manager().add_action([this]() {
        open_ = false;
    });
}

void Window::init() {
    handle_ = SDL_CreateWindow(
        title_.c_str(), width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    glContext = SDL_GL_CreateContext(handle_);
    SDL_GL_MakeCurrent(handle_, glContext);

    IMGUI_CHECKVERSION();
    ctx = ImGui::CreateContext();

    SDL_GL_MakeCurrent(handle_, glContext);
    ImGui::SetCurrentContext(ctx);

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
    SDL_GL_MakeCurrent(handle_, glContext);
    ImGui::SetCurrentContext(ctx);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(handle_);\
    
    spdlog::info("Closed window. Title: {}, id: {}", title_, id_);
}

void Window::update() {
    SDL_GL_MakeCurrent(handle_, glContext);
    ImGui::SetCurrentContext(ctx);

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
}

WindowHandle Window::handle() {
    return handle_;
}

void Window::key_callback(int key) {
    if(key == SDLK_ESCAPE) {
        close();
    }
}

#endif

Window::Window(const std::string &title, int32_t width, int32_t height):
    title_(title),
    id_(random_generator()()),
    centralWidget_(std::move(std::make_unique<Widget>())),
    width_(width),
    height_(height)
{}

uint64_t Window::submit_widget(std::unique_ptr<Widget> widget) {
    auto id = widget->id();
    auto action = [this, widget = std::move(widget)]() mutable {
        glfwMakeContextCurrent(handle_);
        ImGui::SetCurrentContext(ctx);

        if(centralWidget_ != nullptr) {
            centralWidget_->destroy();
        }

        centralWidget_ = std::move(widget);
        centralWidget_->init();
    };
    event_manager().add_action(std::move(action));
    return id;
}

uint64_t Window::set_central_widget(std::unique_ptr<Widget> widget) {
    return submit_widget(std::move(widget));
}

bool Window::is_open() const {
    return open_;
}
