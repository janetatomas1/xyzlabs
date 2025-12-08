
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
        typename... Args
    >
    Widget* set_central_widget(Args... args);
    void set_color(const std::array<float, 4> &color);
    bool export_png(const std::string &filename);
    ImGuiStyle& style();
};

template<typename T>
struct is_unique_ptr_to_widget : std::false_type {};

template<typename T>
struct is_unique_ptr_to_widget<std::unique_ptr<T>>
    : std::bool_constant<std::is_base_of_v<Widget, T>> {};

template<typename T>
inline constexpr bool is_unique_ptr_to_widget_v = is_unique_ptr_to_widget<T>::value;

template<
    WidgetType W,
    typename... Args
>
Widget* Window::set_central_widget(Args... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_widget_v<std::decay_t<First>> && std::is_same_v<W, Widget>) {
            auto widget = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return submit_widget(std::move(widget));
        } else {
            return submit_widget(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                )
            );            
        }
    } else {
        return submit_widget(
            std::make_unique<W>(
                std::forward<Args>(args)...
            )
        );
    }
}

template <typename T>
concept WindowType = std::derived_from<T, Window> || std::same_as<T, Window>;

}
