
#pragma once

#include "xyzlabs/settings/settinginterface.hpp"

namespace xyzlabs {

class SettingBase: public SettingInterface {
protected:
    std::string label_;
public:
    virtual ~SettingBase() = default;
    SettingBase(const std::string &label): label_(label) {};
    virtual const char* label() { return label_.c_str(); };
};

}
