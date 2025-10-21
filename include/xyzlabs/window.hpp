
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <concepts>

#include "xyzlabs/widget.hpp"

class ImGuiContext;

#ifdef USE_GLFW

struct GLFWwindow;
using WindowHandle = GLFWwindow*;

#else

struct SDL_Window;
struct SDL_GLContextState;

using WindowHandle = SDL_Window*;

#endif

class Window {
    #ifndef USE_GLFW
    SDL_GLContextState *glContext;
    #endif
    bool open_ = true;

    WindowHandle handle_ = nullptr;
    ImGuiContext *ctx;

    std::string title_;
    uint64_t id_ = 0;

    int32_t width_ = 0;
    int32_t height_ = 0;

    std::unique_ptr<Widget> centralWidget_;
    uint64_t submit_widget(std::unique_ptr<Widget> widget);    
    void make_context_current();
public:
    void init();
    int32_t width() {
        return width_;
    }
    int32_t height() {
        return height_;
    }
    void close();
    Window(
        const std::string &title = "",
        int32_t width = 0,
        int32_t height = 0
    );
    ~Window();
    void update();
    bool is_open() const;
    WindowHandle handle();
    virtual void key_callback(int key);
    const std::string& title() const {
        return title_;
    }
    uint64_t id() {
        return id_;
    }
    template<WidgetType W = Widget, typename... Args>
    uint64_t set_central_widget(Args... args);
    uint64_t set_central_widget(std::unique_ptr<Widget> widget);
};

template<WidgetType W, typename... Args>
uint64_t Window::set_central_widget(Args... args) {
    auto widget = std::make_unique<W>(std::forward<Args>(args)...);
    return submit_widget(std::move(widget));
}

template <typename T>
concept WindowType = std::derived_from<T, Window> || std::same_as<T, Window>;

#endif
