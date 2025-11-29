
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
        auto leftSidebarSize = size * leftSidebar.sizeRelative;
        auto leftSidebarPos = size * leftSidebar.positionRelative + position;

        centralSize.x -= leftSidebarSize.x;
        centralPos.x += leftSidebarSize.x;

        leftSidebar.ptr->display(leftSidebarSize, leftSidebarPos);
    }

    if(topSidebar.ptr != nullptr && topSidebar.open) {
        auto headerSize = size * topSidebar.sizeRelative;
        auto headerPos = size * topSidebar.positionRelative + position;

        centralSize.y -= headerSize.y;
        centralPos.y = headerSize.y;

        topSidebar.ptr->display(headerSize, headerPos);
    }


    if(rightSidebar.ptr != nullptr && rightSidebar.open) {
        auto rightSidebarSize = size * rightSidebar.sizeRelative;
        auto rightSidebarPos = size * rightSidebar.sizeRelative + position;

        centralSize.x -= rightSidebarSize.x;

        rightSidebar.ptr->display(rightSidebarSize, rightSidebarPos);
    }

    if(bottomSidebar.ptr != nullptr && bottomSidebar.open) {
        auto footerSize = size * bottomSidebar.sizeRelative;
        auto footerPos = size * bottomSidebar.positionRelative + position;

        centralSize.y -= footerSize.y;
        bottomSidebar.ptr->display(footerSize, footerPos);
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

ImVec2 SidebarContainer::get_sidebar_size_relative(Edge edge) {
    return sidebars_[idx(edge)].sizeRelative;
}

ImVec2 SidebarContainer::get_sidebar_position_relative(Edge edge) {
    return sidebars_[idx(edge)].positionRelative;
}

void SidebarContainer::set_sidebar_size_relative(const ImVec2 &size, Edge edge) {
    sidebars_[idx(edge)].sizeRelative = size;
}

void SidebarContainer::set_sidebar_position_relative(const ImVec2 &pos, Edge edge) {
    sidebars_[idx(edge)].positionRelative = pos;
}

}
