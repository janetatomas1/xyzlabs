
#include "xyzlabs/tabwidget.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/eventmanager.hpp"
#include <fmt/format.h>

namespace xyzlabs {

TabWidget::TabWidget(const std::string &title): Widget(title) {}

Widget* TabWidget::add_tab(std::unique_ptr<Widget> tab) {
    Widget *ptr = tab.get();
    event_manager().add_action([this, widget = std::move(tab)]() mutable {
        auto size = btnLayout_.size_relative();
        if(!tabs_.empty()) {
            size.x = size.x * tabs_.size();
        }
        widget->init();
        tabs_.push_back(std::move(widget));
        size.x = size.x / tabs_.size();
        btnLayout_.set_size_relative(size);
    });
    return ptr;
}

void TabWidget::show(const ImVec2 &size, const ImVec2 &position) {    
    for(size_t i=0;i < tabs_.size();i++) {
        auto &tab = tabs_[i];
        auto [btnSize, btnPosition] = btnLayout_.compute(size, position);
        btnPosition.x = btnSize.x * i * (1 + padding);
        ImGui::SetCursorPos(btnPosition);
        if(ImGui::Button(tab->window_id().c_str(), btnSize)) {
            currentTab_ = i;
        }
    }

    if(currentTab_ < tabs_.size()) {
        auto [tabSize, tabPosition] = tabLayout_.compute(size, position);
        auto &currentTab = tabs_[currentTab_];
        currentTab->show(tabSize, tabPosition);
    }
}

}
