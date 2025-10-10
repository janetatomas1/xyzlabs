
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event.hpp"

class WindowManager {
    std::vector<std::unique_ptr<Window>> windows_;
    uint64_t submit_new_window(std::unique_ptr<Window> window);
    inline void flush_closed_windows() {
        std::erase_if(windows_, [](const auto& window) {
            return !window->is_open();
        });
    };
    inline void update_windows() {
        for(auto &w: windows_) {
            w->update();
        }
    };
public:
    void init();
    template<WindowType W = Widget, typename... Args>
    uint64_t add_window(Args... args);
    void update();
    void destroy();
    size_t nwindows() const;
    Window *get_main_window() const;
    Window *get_window_by_id(uint64_t id) const;
    Window *get_window_by_title(const std::string &title) const;
};

template<WindowType W, typename... Args>
uint64_t WindowManager::add_window(Args... args) {
    auto window = std::make_unique<W>(std::forward<Args>(args)...);
    return submit_new_window(std::move(window));
}

#endif
