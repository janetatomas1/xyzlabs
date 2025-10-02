
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event.hpp"

class WindowHandle;

class WindowManager {
    std::vector<std::unique_ptr<Window>> windows_;
    uint64_t submit_new_window(std::unique_ptr<Window> window);
public:
    void init();
    template<WindowType W = Widget, typename... Args>
    uint64_t add_window(Args... args);
    uint64_t add_window(std::unique_ptr<Window> window);
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
