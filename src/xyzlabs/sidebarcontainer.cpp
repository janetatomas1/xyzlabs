
#include "xyzlabs/sidebarcontainer.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/eventmanager.hpp"

namespace xyzlabs { 

size_t idx(Edge e) {
    return static_cast<size_t>(e);
}

SidebarContainer::SidebarContainer(const std::string &title): Widget(title) {}

void SidebarContainer::display(const ImVec2 &size, const ImVec2& position) {
    ImVec2 centralSize = size;
    ImVec2 centralPos = position;
    
    auto &leftSidebar = sidebars_[idx(Edge::Left)];
    auto &topSidebar = sidebars_[idx(Edge::Top)];
    auto &rightSidebar = sidebars_[idx(Edge::Right)];
    auto &bottomSidebar = sidebars_[idx(Edge::Bottom)];

    if(leftSidebar.ptr != nullptr && leftSidebar.open) {
        auto [localSize, localPos] = leftSidebar.ptr->layout().compute(size, position);

        centralSize.x -= localSize.x;
        centralPos.x += localSize.x;

        leftSidebar.ptr->display(size, position);
    }

    if(topSidebar.ptr != nullptr && topSidebar.open) {
        auto [localSize, localPos] = topSidebar.ptr->layout().compute(size, position);

        centralSize.y -= localSize.y;
        centralPos.y = localSize.y;

        topSidebar.ptr->display(size, position);
    }


    if(rightSidebar.ptr != nullptr && rightSidebar.open) {
        auto [localSize, localPos] = rightSidebar.ptr->layout().compute(size, position);

        centralSize.x -= localSize.x;

        rightSidebar.ptr->display(size, position);
    }

    if(bottomSidebar.ptr != nullptr && bottomSidebar.open) {
        auto [localSize, localPos] = bottomSidebar.ptr->layout().compute(size, position);

        centralSize.y -= localSize.y;
        bottomSidebar.ptr->display(size, position);
    }
    
    Widget::display(centralSize, centralPos);
}

Widget* SidebarContainer::get_sidebar(Edge edge) {
    return sidebars_[idx(edge)].ptr.get();
}

void SidebarContainer::set_sidebar(std::unique_ptr<Widget> widget, Edge edge) {
    action act = [this, widget = std::move(widget), edge]() mutable {
        sidebars_[idx(edge)].ptr = std::move(widget);
        sidebars_[idx(edge)].open = true;
    };

    event_manager().add_action(std::move(act));
}

}
