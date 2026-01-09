
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "widget.hpp"
#include "xyzlabs/widget.hpp"

namespace xyzlabs {

class TabWidget: public Widget {
    std::vector<std::unique_ptr<Widget>> tabs_;
    std::string tabBarID_;
    float padding = 0.005;
    size_t currentTab_ = 0;

    ImVec2 closeBtnSize = {30.0f, 0.0f};
    ImVec2 closeBtnPosition = {0.0f, 0.0f};
    ImVec2 btnSize = {0.0f, 0.0f};
    ImVec2 btnPosition = {0.0f, 0.0f};

    RelativeLayout btnLayout_ = {{
        1.0f, 0.035f
    }, {
       0.0f, 0.0f
    }};
    RelativeLayout closeBtnLayout_ = {{
        0.02f, 1.0f
    }, {
       0.98f, 0.0f
    }};
    RelativeLayout tabLayout_ = {{
        1.0f, 0.965f
    }, {
       0.0f, 0.035f
    }};
    Widget* add_tab_internal(std::unique_ptr<Widget> tab, size_t position = std::string::npos);
    Widget* set_tab_internal(std::unique_ptr<Widget> tab, size_t position);
    void render_full_tabbar();
    void render_only_current();
    void recompute_tab_width();
public:
    TabWidget(
        const std::string &title = "",
        Widget *parent = nullptr,
        Window *window = nullptr
    );
    void show(const ImVec2 &size, const ImVec2 &position) override;
    template<
        WidgetType W = Widget,
        typename... Args
    >
    Widget* add_tab(Args&&... args);
    template<
        WidgetType W = Widget,
        typename... Args
    >
    Widget* set_tab(size_t position, Args&&... args);
    Widget* get_tab(size_t idx);
    Widget* get_tab_id(uint64_t id);
    Widget* get_current_tab();
    size_t get_current_index();
    size_t count();
    void remove_tab(size_t idx);
    void remove_tab_id(uint64_t id);
};

template<
    WidgetType W,
    typename... Args
>
Widget* TabWidget::add_tab(Args&&... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_widget_v<std::decay_t<First>> && std::is_same_v<W, Widget>) {
            auto tab = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return add_tab_internal(
                std::move(tab)
            );
        } else {
            return add_tab_internal(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                )
            );
        }
    } else {
        auto tab = std::make_unique<W>(std::forward<Args>(args)...);
        return add_tab_internal(
            std::move(tab)
        );
    }
};

template<
    WidgetType W,
    typename... Args
>
Widget* TabWidget::set_tab(size_t position, Args&&... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_widget_v<std::decay_t<First>> && std::is_same_v<W, Widget>) {
            auto tab = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return set_tab_internal(std::move(tab), position);
        } else {
            return set_tab_internal(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                ),
                position
            );
        }
    } else {
        auto tab = std::make_unique<W>(std::forward<Args>(args)...);
        return set_tab_internal(std::move(tab), position);
    }
};

}
