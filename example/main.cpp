
#include "xyzlabs/globals.hpp"
#include "burningforest.hpp"
#include "xyzlabs/tabwidget.hpp"

using namespace xyzlabs;

class A: public TabWidget {
public:
    A(): TabWidget(){};
    void show(const ImVec2 &size, const ImVec2 &position) override {
        TabWidget::show(size, position);
        if(ImGui::IsKeyDown(ImGuiKey_N)) {
            add_tab(std::make_unique<BurningForest>(), 0);
        }

        if(ImGui::IsKeyDown(ImGuiKey_M)) {
            add_tab<Widget>(1);
        }
    }
};

int main() {
    app().init<Window, A>("Burning forest");
    return app().exec();
}
