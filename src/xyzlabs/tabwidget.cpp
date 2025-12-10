
#include "xyzlabs/tabwidget.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/window.hpp"

namespace xyzlabs {

TabWidget::TabWidget(
    const std::string &title,
    Widget *parent,
    Window *window
): Widget(title, parent, window) {}

Widget* TabWidget::add_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    Widget *ptr = tab.get();
    event_manager().add_action([this, position, widget = std::move(tab)]() mutable {
        auto size = btnLayout_.size_relative();
        widget->set_window(window());
        widget->set_parent(this);
        window()->make_context_current();
        widget->init();
        if(position < tabs_.size()) {
            tabs_.insert(tabs_.begin() + position, std::move(widget));
            currentTab_ = position;
        } else {
            tabs_.push_back(std::move(widget));
            currentTab_ = tabs_.size() - 1;
        }
        if(tabs_.size() > 1) {
            size.x = size.x * (tabs_.size() - 1);
        }
        size.x = size.x / tabs_.size();
        btnLayout_.set_size_relative(size);
    });
    return ptr;
}

Widget* TabWidget::set_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    if(tabs_.size() > position) {
        Widget *ptr = tab.get();
        event_manager().add_action([this, position, tab = std::move(tab)]() mutable {
            window()->make_context_current();
            tab->init();
            tab->set_parent(this);
            tab->set_window(window());
            tabs_[position]->destroy();
            tabs_[position] = std::move(tab);
        });
        return ptr;
    }

    return nullptr;
}

void TabWidget::show(const ImVec2 &size, const ImVec2 &position) {
    if(tabs_.size() > 1) {
        for(size_t i=0;i < tabs_.size();i++) {
            auto &tab = tabs_[i];
            auto [btnSize, btnPosition] = btnLayout_.compute(size, position);
            btnPosition.x = btnSize.x * i * (1 + padding);
            ImGui::SetCursorPos(btnPosition);
            if(ImGui::Button(tab->window_id().c_str(), btnSize)) {
                currentTab_ = i;
            }
        }
    }

    if(currentTab_ < tabs_.size()) {
        auto [tabSize, tabPosition] = tabLayout_.compute(size, position);
        auto &currentTab = tabs_[currentTab_];
        currentTab->show(tabSize, tabPosition);
    }
}

Widget* TabWidget::get_tab(size_t idx) {
    return tabs_[idx].get();
}

Widget* TabWidget::get_tab_id(uint64_t id) {
    Widget *ptr = nullptr;
    for(auto &tab: tabs_) {
        if(tab->id() == id) {
            ptr = tab.get();
        }
    }

    return ptr;
}

Widget* TabWidget::get_current_tab() {
    return tabs_[currentTab_].get();
}

size_t TabWidget::get_current_index() {
    return currentTab_;
}

size_t TabWidget::count() {
    return tabs_.size();
}

void TabWidget::remove_tab(size_t idx) {
    event_manager().add_action([this, idx] () {
        tabs_[idx]->destroy();
        tabs_.erase(tabs_.begin() + idx);
    });
}

void TabWidget::remove_tab_id(uint64_t id) {
    event_manager().add_action([this, id]() {
        for(size_t i=0;i < tabs_.size();i++) {
            if(tabs_[i]->id() == id) {
                tabs_[id]->destroy();
                tabs_.erase(tabs_.begin() + i);
                return;
            }
        }
    });
}

}
