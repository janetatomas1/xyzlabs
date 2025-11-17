
#pragma once

#include "xyzlabs/floatingwidget.hpp"

namespace xyzlabs {

class Dialog: public FloatingWidget {
protected:
    std::string acceptBtnText_ = "OK";
    std::string text_ = "";
public:
    Dialog(const std::string &text, const std::string title, const std::string &acceptBtnText = "");
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    virtual void accept_callback() {};
    virtual void reject_callback() {};
    void set_text(const std::string &text);
    void set_accept_btn_text(const std::string &text);
};

}
