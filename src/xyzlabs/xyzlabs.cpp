
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <stdexcept>
#include <cstdlib>
#include <fmt/format.h>

#include "xyzlabs/utils.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"


void XYZLabs::init_() {
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

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), NULL, NULL);
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

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if(widgetManager_.nwidgets() == 0) {
        set_initial_widget<DefaultIntroWidget>();
    }

    create_app_directory();
    taskManager_.run();
    widgetManager_.init();
    settingsManager_.init();
}

void XYZLabs::mainloop_() {
    while(true) {
        eventManager_.dispatch();
        glfwPollEvents();

        if(glfwGetKey(window_, GLFW_KEY_ESCAPE)) {
            spdlog::info("ESC key pressed, closing!");
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        if(glfwWindowShouldClose(window_)) {
            break;
        }

        glfwGetWindowSize(window_, &width_, &height_);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();    
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();
        widgetManager_.show({static_cast<float>(width_), static_cast<float>(height_)});

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
        glfwSwapBuffers(window_);
    }
}

void XYZLabs::exit_() {
    taskManager_.stop();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    glfwTerminate();
    spdlog::info("Closed {}", title_);
}

int XYZLabs::exec() {
    try {
        init_();
        mainloop_();
        exit_();
    } catch(std::exception &e) {
        exitCode_ = 1;
    }
    return exitCode_;
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

RandomGenerator &XYZLabs::random_generator() {
    return randomGenerator_;
}

SettingsManager &XYZLabs::settings_manager() {
    return settingsManager_;
}

EventManager& XYZLabs::event_manager() {
    return eventManager_;
}

void XYZLabs::close() {
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

XYZLabs& XYZLabs::init(const std::string &title) {
    title_ = title;
    return instance();
}

const std::string& XYZLabs::title() {
    return title_;
}

std::filesystem::path XYZLabs::app_directory() {
    auto titleStandardized = utils::standardize(title());
    
    #ifdef __unix__
        return std::filesystem::path(std::getenv("HOME")) / fmt::format(".{}", titleStandardized);
    #endif
}

std::filesystem::path XYZLabs::create_app_directory() {
    auto appDirectory = app_directory();

    if(!std::filesystem::is_directory(appDirectory)) {
        std::filesystem::create_directory(appDirectory);
    }

    return appDirectory;
}
