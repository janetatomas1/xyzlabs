
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <concepts>

#include "xyzlabs/widget.hpp"

class ImGuiContext;
class GLFWwindow;

class Window {
    GLFWwindow *handle_ = nullptr;
    ImGuiContext *ctx;

    std::string title_;
    uint64_t id_ = 0;

    int32_t width_ = 1000;
    int32_t height_ = 1000;

    std::unique_ptr<Widget> centralWidget_;
    void init();
public:
    int32_t width() {
        return width_;
    }
    int32_t height() {
        return height_;
    }
    template<WidgetType W = Widget, typename... Args>
    Window(const std::string &title = "", Args... args);
    ~Window();
    void update();
    bool should_close() const;
    GLFWwindow* handle();
    virtual void key_callback(int key);
    void reset_key_callback();
    const std::string& title() const {
        return title_;
    }
    uint64_t id() {
        return id_;
    }
    template<WidgetType W = Widget, typename... Args>
    uint64_t set_central_widget(Args... args);
};

template<WidgetType W, typename... Args>
Window::Window(const std::string &title, Args... args):
    title_(title) {
    init();
    set_central_widget<W>(std::forward<Args>(args)...);
}

template<WidgetType W, typename... Args>
uint64_t Window::set_central_widget(Args... args) {
    centralWidget_ = std::make_unique<W>(std::forward<Args>(args)...);
    return centralWidget_->id();
}

template <typename T>
concept WindowType = std::derived_from<T, Window> || std::same_as<T, Window>;

#endif
