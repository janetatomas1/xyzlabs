
#ifndef DEFAULTINTROWIDGET_HPP
#define DEFAULTINTROWIDGET_HPP

#include "xyzlabs/widget.hpp"

class DefaultIntroWidget: public Widget {
public:
    void show(const ImVec2 &size, const ImVec2& position) override;
    DefaultIntroWidget();
};

#endif
