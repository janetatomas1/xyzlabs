
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"

class WindowHandle;

class WindowManager {
	std::vector<std::unique_ptr<Window>> windows_;
public:
    void init();
    void add_window();
    void update();
    void destroy();
    size_t nwindows();
};

#endif
