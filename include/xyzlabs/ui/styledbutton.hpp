
#pragma once

#include "xyzlabs/ui/widget.hpp"

namespace xyzlabs {

class StyledButton : public Widget {
    ImVec4 btnColor_;
    ImVec4 btnHoverColor_;
    ImVec4 btnActiveColor_;
    ImVec4 btnTextColor_;

public:
    StyledButton(const std::string& title = "");
    bool draw(const ImVec2& size, const ImVec2& pos);
    bool operator()(const ImVec2& size, const ImVec2& pos);
    ImVec4& color();
    ImVec4& hover_color();
    ImVec4& active_color();
    ImVec4& text_color();
};

}
