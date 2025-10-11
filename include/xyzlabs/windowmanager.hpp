
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event.hpp"
#include "xyzlabs/globals.hpp"

class WindowManager {
    std::vector<std::unique_ptr<Window>> windows_;

    template<WindowType W = Window, typename... Args>
    uint64_t submit_new_window(Args... args);
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
    void add_window_action(action act);
public:
    void init();
    template<WindowType W = Window, typename... Args>
    uint64_t add_window(Args... args);
    void update();
    void destroy();
    size_t nwindows() const;
    Window *get_main_window() const;
    Window *get_window_by_id(uint64_t id) const;
    Window *get_window_by_title(const std::string &title) const;
};

template<WindowType W, typename... Args>
uint64_t WindowManager::submit_new_window(Args... args) {
    auto id = random_generator().random();
    auto action = [this, id] () mutable {
        auto window = std::make_unique<W>();
        window->set_id(id);
        windows_.push_back(std::move(window));
    };
    add_window_action(std::move(action));
    return id;
} 


template<WindowType W, typename... Args>
uint64_t WindowManager::add_window(Args... args) {
    return submit_new_window<W, Args...>(std::forward<Args>(args)...);
}

#endif
