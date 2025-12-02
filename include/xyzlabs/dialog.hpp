
#pragma once

#include "xyzlabs/widget.hpp"
#include "xyzlabs/event.hpp"

namespace xyzlabs {

class Dialog: public Widget {
    std::string content_;
    std::string acceptText_ = "OK";
    std::string rejectText_ = "Cancel";
  
    action acceptAction_ = []() {};
    action rejectAction_ = []() {};

    ImVec4 color_ = {
        0.0f, 0.0f, 0.0f, 1.0f
    };
    bool rejectActive_ = true;

    RelativeLayout contentLayout_ = {
        {0.8f, 0.6f},
        {0.1f, 0.1f}
    };
    RelativeLayout acceptBtnLayout_ = {
        {0.15f, 0.12f},
        {0.6f, 0.8f}
    };
    RelativeLayout rejectBtnLayout_ = {
        {0.15f, 0.12f},
        {0.2f, 0.8f}
    };
public:
    Dialog(
        const std::string &content,
        const std::string &acceptText = "OK",
        const std::string &rejectText = "Cancel",
        const std::string &title = ""
    );
    void display(const ImVec2 &size, const ImVec2 &position) override;
    void show(const ImVec2 &size, const ImVec2 &position) override;
    virtual void accept();
    virtual void reject();
    void set_content(const std::string &text);
    void set_accept_text(const std::string &text);
    void set_reject_text(const std::string &text);
    std::string content();
    std::string accept_text();
    std::string reject_text();
    void set_accept_action(action act);
    void set_reject_action(action act);
    ImVec4 background_color();
    void set_background_color(const ImVec4 &color);
};

}
