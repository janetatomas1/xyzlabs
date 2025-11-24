
#include "xyzlabs/sidebarcontainer.hpp"

namespace xyzlabs { 

void SidebarContainer::display(const ImVec2 &size, const ImVec2& position) {
    ImVec2 centralSize = size;
    ImVec2 centralPos = position;
    
    if(leftSidebar_ != nullptr && leftSidebarOpen_) {
        auto leftSidebarSize = size * leftSidebarSizeRelative_;
        auto leftSidebarPos = size * leftSidebarPositionRelative_ + position;

        centralSize.x -= leftSidebarSize.x;
        centralPos.x += leftSidebarSize.x;

        leftSidebar_->display(leftSidebarSize, leftSidebarPos);
    }

    if(rightSidebar_ != nullptr && rightSidebarOpen_) {
        auto rightSidebarSize = size * rightSidebarSizeRelative_;
        auto rightSidebarPos = size * rightSidebarPositionRelative_ + position;

        centralSize.x -= rightSidebarSize.x;

        rightSidebar_->display(rightSidebarSize, rightSidebarPos);
    }

    if(header_ != nullptr && headerOpen_) {
        auto headerSize = size * headerSizeRelative_;
        auto headerPos = size * headerPositionRelative_ + position;

        centralSize.y -= headerSize.y;
        centralPos.y = headerSize.y;

        header_->display(headerSize, headerPos);
    }

    if(footer_ != nullptr && footerOpen_) {
        auto footerSize = size * footerSizeRelative_;
        auto footerPos = size * footerPositionRelative_ + position;

        centralSize.y -= footerSize.y;
        footer_->display(footerSize, footerPos);
    }
    
    Widget::display(centralSize, centralPos);
}

void SidebarContainer::set_sidebar(std::unique_ptr<Widget> widget, Edge edge) {
    switch (edge) {
        case Edge::Left: {
            leftSidebar_ = std::move(widget);
            leftSidebarOpen_ = true;
            break;
        };
        case Edge::Right: {
            rightSidebar_ = std::move(widget);
            rightSidebarOpen_ = true;
            break;
        }
        case Edge::Top: {
            header_ = std::move(widget);
            headerOpen_ = true;
            break;
        }
        case Edge::Bottom: {
            footer_ = std::move(widget);
            footerOpen_ = true;
            break;
        }
        default:
            break;
    }
}

}
