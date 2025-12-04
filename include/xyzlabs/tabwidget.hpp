
#pragma once

#include <memory>
#include <vector>

#include "xyzlabs/widget.hpp"

namespace xyzlabs {

class TabWidget: public Widget {
    std::vector<std::unique_ptr<Widget>> tabs_;
    float padding = 0.005;
    size_t currentTab_ = 0;

    RelativeLayout btnLayout_ = {{
        1.0f - padding, 0.035f
    }, {
       0.0f, 0.0f 
    }};
    RelativeLayout tabLayout_ = {{
        1.0f, 0.965f
    }, {
       0.0f, 0.035f
    }};

public:
    TabWidget(const std::string &title = "");
    Widget* add_tab(std::unique_ptr<Widget> tab);
    void show(const ImVec2 &size, const ImVec2 &position) override;
};

}
