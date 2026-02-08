
#pragma once

#include "xyzlabs/ui/widget.hpp"

namespace xyzlabs {

class Button: public Widget {
public:
    Button(const std::string& title = "");
    bool draw(const ImVec2& size, const ImVec2& pos);
    bool operator()(const ImVec2& size, const ImVec2& pos);
};


}
