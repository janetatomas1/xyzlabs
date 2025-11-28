
#include <memory>
#include <array>

#include "xyzlabs/widget.hpp"

namespace xyzlabs {

enum class Edge {
    Left = 0,
    Top = 1,
    Right = 2,
    Bottom =3
};

class SidebarContainer: public Widget {
    struct SidebarInfo {
        std::unique_ptr<Widget> ptr = nullptr;
        ImVec2 sizeRelative = {0.0f, 0.0f};
        ImVec2 positionRelative = {0.0f, 0.0f};
        bool open = false;

        SidebarInfo() = default;
        SidebarInfo(const SidebarInfo&) = delete;
        SidebarInfo& operator=(const SidebarInfo&) = delete;
        SidebarInfo(SidebarInfo&&) = default;
        SidebarInfo& operator=(SidebarInfo&&) = default;
    };
    std::array<SidebarInfo, 4> sidebars_;

public:
    SidebarContainer(const std::string &title = ""): Widget(title) {};
    void display(const ImVec2 &size, const ImVec2& position) override;
    Widget* get_sidebar(Edge edge = Edge::Left);
    void set_sidebar(std::unique_ptr<Widget> widget, Edge edge = Edge::Left);
    ImVec2 get_sidebar_size_relative(Edge edge = Edge::Left);
    ImVec2 get_sidebar_position_relative(Edge edge = Edge::Left);
    void set_sidebar_size_relative(const ImVec2 &size, Edge edge = Edge::Left);
    void set_sidebar_position_relative(const ImVec2 &pos, Edge edge = Edge::Left);
};

}
