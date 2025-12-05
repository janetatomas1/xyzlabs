
#pragma once

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

namespace xyzlabs {

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
    Widget* submit_widget(std::unique_ptr<Widget> widget);    
public:
    void make_context_current();
    virtual void init();
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
    virtual void key_callback();
    const std::string& title() const {
        return title_;
    }
    uint64_t id() {
        return id_;
    }
    template<
        WidgetType W = Widget,
        typename... Args,
        typename = std::enable_if_t<
            !((sizeof...(Args) == 0) ||(sizeof...(Args) == 1 &&
            std::is_convertible_v<std::decay_t<Args>..., std::unique_ptr<Widget>>
            ))
        >
    >
    Widget* set_central_widget(Args... args) {
        auto widget = std::make_unique<W>(std::forward<Args>(args)...);
        return submit_widget(std::move(widget));
    }
    template<WidgetType W = Widget>
    Widget* set_central_widget() {
        auto widget = std::make_unique<W>();
        return submit_widget(std::move(widget));
    }
    Widget* set_central_widget(std::unique_ptr<Widget> widget);
    void set_color(const std::array<float, 4> &color);
    bool export_png(const std::string &filename);
    ImGuiStyle& style();
};

template <typename T>
concept WindowType = std::derived_from<T, Window> || std::same_as<T, Window>;

}
