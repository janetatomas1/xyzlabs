
#include <GL/glew.h>

#include <imgui_impl_opengl3.h>
#include <imgui.h>

#include "xyzlabs/window.hpp"
#include "xyzlabs/xyzlabs.hpp"


constexpr ImGuiWindowFlags WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;

#ifdef USE_GLFW

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

void Window::init() {
    id_ = XYZLabs::instance().random_generator().random();
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
    glfwSetWindowUserPointer(handle_, this);

    glfwSetKeyCallback(handle_, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
        Window *win = (Window*)glfwGetWindowUserPointer(handle);
        win->key_callback(key);
    });
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

    ImVec2 size = {static_cast<float>(width_), static_cast<float>(height_)};
    ImVec2 pos = {0.0f, 0.0f};
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    if(ImGui::Begin("##window", nullptr,  WINDOW_FLAGS)) {
        centralWidget_->show(size, pos);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
    glfwSwapBuffers(handle_);
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle_) != 0;
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
    XYZLabs::instance().event_manager().add_action([this]() {
        SDL_DestroyWindow(handle_);
        open_ = false;
    });
}

void Window::init() {
    handle_ = SDL_CreateWindow(
        title_.c_str(), width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    glContext = SDL_GL_CreateContext(handle_);
    SDL_GL_MakeCurrent(handle_, glContext);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ctx = ImGui::CreateContext();

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
}

Window::~Window() {
}

void Window::update() {
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
        ImGui::End();
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(handle_);
}

bool Window::should_close() const {
    return !open_;
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

uint64_t Window::submit_widget(std::unique_ptr<Widget> widget) {
    auto id = widget->id();
    auto action = [this, widget = std::move(widget)]() mutable {
        centralWidget_ = std::move(widget);
    };
    XYZLabs::instance().event_manager().add_action(std::move(action));
    return id;
}

uint64_t Window::set_central_widget(std::unique_ptr<Widget> widget) {
    return submit_widget(std::move(widget));
}
