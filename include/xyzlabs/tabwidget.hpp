
#pragma once

#include <memory>
#include <vector>

#include "xyzlabs/widget.hpp"

namespace xyzlabs {

class TabWidget: public Widget {
    std::vector<std::unique_ptr<Widget>> tabs_;
    float padding = 0.005;
    size_t currentTab_ = 0;

    RelativeLayout btnLayout_ = {{
        1.0f - padding, 0.035f
    }, {
       0.0f, 0.0f 
    }};
    RelativeLayout tabLayout_ = {{
        1.0f, 0.965f
    }, {
       0.0f, 0.035f
    }};

public:
    TabWidget(
        const std::string &title = "",
        Widget *parent = nullptr,
        Window *window = nullptr
    );
    void show(const ImVec2 &size, const ImVec2 &position) override;
    Widget* add_tab(std::unique_ptr<Widget> widget, size_t position = std::string::npos);
    template<
        WidgetType W = Widget,
        typename... Args,
        typename = std::enable_if_t<
            !((sizeof...(Args) == 0) ||(sizeof...(Args) == 1 &&
            std::is_convertible_v<std::decay_t<Args>..., std::unique_ptr<Widget>>
            ))
        >
    >
    Widget* add_tab(size_t position = std::string::npos, Args&&... args) {
        auto tab = std::make_unique<W>(std::forward<Args>(args)...);
        return add_tab(std::move(tab));
    };
    template<WidgetType W = Widget>
    Widget* add_tab(size_t position = std::string::npos) {
        auto tab = std::make_unique<W>();
        return add_tab(std::move(tab), position);
    };
    Widget* get_tab(size_t idx);
    Widget* get_tab_id(uint64_t id);
    Widget* get_current_tab();
    size_t get_current_index();
    size_t count();
    void remove_tab(size_t idx);
    void remove_tab_id(uint64_t id);
};

}
