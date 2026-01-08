
#pragma once

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event.hpp"
#include "xyzlabs/eventmanager.hpp"


namespace xyzlabs {

class XYZLabs;

class WindowManager {
    std::vector<std::unique_ptr<Window>> windows_;
    int64_t currentWindowIDx_ = -1;

    inline void flush_closed_windows() {
        std::erase_if(windows_, [](const auto& window) {
            return !window->is_open();
        });
    };
    inline void update_windows() {
        for(currentWindowIDx_ = 0;currentWindowIDx_ < windows_.size();currentWindowIDx_++) {
            windows_[currentWindowIDx_]->update();
        }
    };
    Window* submit_window(std::unique_ptr<Window> window);
    XYZLabs* app_;
public:
    WindowManager(XYZLabs* app);
    void init_main_window(std::unique_ptr<Window> window);
    void init();
    template<WindowType W = Window, typename... Args>
    Window* add_window(Args... args);
    void update();
    void destroy();
    size_t nwindows() const;
    Window *get_main_window() const;
    Window *get_window_by_id(uint64_t id) const;
    Window *get_window_by_title(const std::string &title) const;
    Window *get_current_window();
    XYZLabs* app();
};

template<typename T>
struct is_unique_ptr_to_window : std::false_type {};

template<typename T>
struct is_unique_ptr_to_window<std::unique_ptr<T>>
    : std::bool_constant<std::is_base_of_v<Window, T>> {};

template<typename T>
inline constexpr bool is_unique_ptr_to_window_v = is_unique_ptr_to_window<T>::value;

template<WindowType W, typename... Args>
Window* WindowManager::add_window(Args... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_window_v<std::decay_t<First>> && std::is_same_v<W, Window>) {
            auto window = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return submit_window(std::move(window));
        } else {
            return submit_window(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                )
            );
        }
    } else {
        return submit_window(
            std::make_unique<W>(
                std::forward<Args>(args)...
            )
        );
    }
}

}
