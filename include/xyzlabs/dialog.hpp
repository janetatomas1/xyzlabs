
#pragma once

#include "xyzlabs/widget.hpp"
#include "xyzlabs/event.hpp"


namespace xyzlabs {

class Dialog: public Widget {
    std::string acceptText_ = "OK";
    std::string rejectText_ = "Cancel";
  
    ImVec2 sizeRelative = {0.0f, 0.0f};
    ImVec2 positionRelative = {0.0f, 0.0f};
  
    action acceptAction_;
    action rejectAction_;
public:
    Dialog(const std::string &widget = "");
    void display(const ImVec2 &size, const ImVec2 &position);
    virtual void accept();
    virtual void reject();
    void set_accept_text(const std::string &text);
    void set_reject_text(const std::string &text);
    std::string get_accept_text();
    std::string get_reject_text();
    void set_accept_action(action act);
    void set_reject_action(action act);
    ImVec2 get_sidebar_size_relative();
    ImVec2 get_sidebar_position_relative();
    void set_sidebar_size_relative(const ImVec2 &size);
    void set_sidebar_position_relative(const ImVec2 &pos);
};

}
