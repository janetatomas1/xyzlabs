
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

    template<WindowType W = Window, WidgetType Wid = Widget, typename... Args>
    uint64_t submit_new_window(Args... args);
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
public:
    void init();
    template<WindowType W = Window, WidgetType Wid = Widget, typename... Args>
    uint64_t add_window(Args... args);
    void update();
    void destroy();
    size_t nwindows() const;
    Window *get_main_window() const;
    Window *get_window_by_id(uint64_t id) const;
    Window *get_window_by_title(const std::string &title) const;
    Window *get_current_window();
};

template<WindowType W, WidgetType Wid, typename... Args>
uint64_t WindowManager::submit_new_window(Args... args) {
    auto id = random_generator().random();
    event_manager().add_action(std::move([this, id] () mutable {
        auto window = std::make_unique<W>();
        window->set_id(id);
        windows_.push_back(std::move(window));
    }));

    event_manager().add_action(std::move([this, id, ...args = std::move(args)] () {
        auto window = get_window_by_id(id);
        std::unique_ptr<Widget> widget = std::make_unique<Wid>(std::forward(args)...);
        window->set_central_widget(std::move(widget));
    }));

    return id;
} 

template<WindowType W, WidgetType Wid, typename... Args>
uint64_t WindowManager::add_window(Args... args) {
    return submit_new_window<W, Wid, Args...>(std::forward<Args>(args)...);
}

#endif
