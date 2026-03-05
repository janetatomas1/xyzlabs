
#pragma once

#include "xyzlabs/ui/widget.hpp"
#include "xyzlabs/event/event.hpp"
#include "xyzlabs/ui/button.hpp"

namespace xyzlabs {

class Dialog: public Widget {
    std::string content_;

    Button rejectButton_;
    Button acceptButton_;

    action acceptAction_ = []() {};
    action rejectAction_ = []() {};

    ImVec4 color_ = {
        0.0f, 0.0f, 0.0f, 1.0f
    };
    bool rejectActive_ = true;

    RelativeLayout contentLayout_ = {
        {0.3f, 0.2f},
        {0.1f, 0.1f}
    };
public:
    Dialog(
        const std::string &content,
        const std::string &acceptText = "OK",
        const std::string &rejectText = "Cancel",
        const std::string &title = "",
        Widget *parent = nullptr,
        Window *window = nullptr
    );
    void show(const ImVec2 &size, const ImVec2 &position) override;
    virtual void accept();
    virtual void reject();
    void set_content(const std::string &text);
    std::string content();
    void set_accept_action(action act);
    void set_reject_action(action act);
    ImVec4& background_color();
    void set_reject_active(bool value);
    RelativeLayout &content_layout();
    Button &accept_button();
    Button &reject_button();
};

}
