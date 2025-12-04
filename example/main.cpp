
#include "xyzlabs/globals.hpp"
#include "burningforest.hpp"
#include "xyzlabs/tabwidget.hpp"

using namespace xyzlabs;

class A: public TabWidget {
public:
    A(): TabWidget(){
        add_tab<BurningForest>();
        add_tab(std::make_unique<Widget>("hello www"));
        add_tab(std::make_unique<Widget>("aaaaaa"));
        add_tab(std::make_unique<BurningForest>());
        add_tab(std::make_unique<Widget>("11111"));
    };
};

int main() {
    app().init<Window, A>("Burning forest");
    return app().exec();
}
