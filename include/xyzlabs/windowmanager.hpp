
#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"

using WindowHandle = std::unique_ptr<Window>;

class WindowManager {
public:
	std::vector<WindowHandle> windows_;

    void init();
    void add_window();
    void update();
    void destroy();
};

#endif
