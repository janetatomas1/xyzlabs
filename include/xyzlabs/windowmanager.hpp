
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/eventmanager.hpp"


class WindowManager {
    std::vector<std::unique_ptr<Window>> windows_;
    size_t renderTimeout_ = 30;
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
    void init_main_window();
public:
    void init();
    template<WindowType W = Window, typename... Args>
    Window* add_window(Args... args);
    Window* add_window(std::unique_ptr<Window> window);
    void update();
    void destroy();
    size_t nwindows() const;
    Window *get_main_window() const;
    Window *get_window_by_id(uint64_t id) const;
    Window *get_window_by_title(const std::string &title) const;
    Window *get_current_window();
};

template<WindowType W, typename... Args>
Window* WindowManager::add_window(Args... args) {
    auto window = std::make_unique<W>(std::forward<Args>(args)...);
    return add_window(std::move(window));
}

#endif
