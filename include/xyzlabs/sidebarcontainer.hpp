
#include <memory>

#include "xyzlabs/widget.hpp"

namespace xyzlabs {

enum class Edge {
    Left,
    Right,
    Top,
    Bottom
};

class SidebarContainer: public Widget {
protected:
    std::unique_ptr<Widget> leftSidebar_ = nullptr;
    std::unique_ptr<Widget> rightSidebar_ = nullptr;
    std::unique_ptr<Widget> header_ = nullptr;
    std::unique_ptr<Widget> footer_ = nullptr;
    
    ImVec2 leftSidebarSizeRelative_ = {0.07f, 1.0f};
    ImVec2 leftSidebarPositionRelative_ = {0.0f, 0.0f};

    ImVec2 rightSidebarSizeRelative_ = {0.07f, 1.0f};
    ImVec2 rightSidebarPositionRelative_ = {0.093f, 1.0f};

    ImVec2 headerSizeRelative_ = {1.0f, 0.3f};
    ImVec2 headerPositionRelative_ = {0.0f, 0.0f};

    ImVec2 footerSizeRelative_ = {1.0f, 0.1f};
    ImVec2 footerPositionRelative_ = {0.9f, 0.0f};

    bool leftSidebarOpen_ = false;
    bool rightSidebarOpen_ = false;
    bool headerOpen_ = false;
    bool footerOpen_ = false;
public:
    SidebarContainer(const std::string &title = ""): Widget(title) {};
    void display(const ImVec2 &size, const ImVec2& position) override;
    void set_sidebar(std::unique_ptr<Widget> widget, Edge edge = Edge::Left);
};

}
