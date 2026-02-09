#pragma once

#include "xyzlabs/ui/widget.hpp"

namespace xyzlabs {

/**
 * @brief Button with locally overridden visual style.
 *
 * StyledButton is a wrapper around ImGui::Button that allows per-widget
 * customization of colors without modifying the global ImGui style.
 *
 * It is intended for buttons that should visually stand out from the rest
 * of the application (e.g. primary actions, destructive actions, emphasis
 * controls). For buttons that follow the application-wide theme, prefer
 * using the global ImGui style instead.
 *
 * The style is applied using the ImGui style stack and is scoped to the
 * draw() call only.
 */
class StyledButton : public Widget {
    ImVec4 btnColor_;
    ImVec4 btnHoverColor_;
    ImVec4 btnActiveColor_;
    ImVec4 btnTextColor_;

public:
    /**
     * @brief Construct a styled button with an optional title.
     *
     * @param title Button label (default: empty).
     */
    explicit StyledButton(const std::string& title = "");

    /**
     * @brief Render the button with its local style overrides.
     *
     * Computes layout using the associated layout object, applies the
     * configured colors via the ImGui style stack, and renders the button.
     *
     * @param size Available size for layout computation.
     * @param pos  Parent-relative position for layout computation.
     * @return true if the button was clicked during this frame.
     */
    bool draw(const ImVec2& size, const ImVec2& pos);

    /**
     * @brief Convenience call operator.
     *
     * Equivalent to calling draw(size, pos).
     *
     * @param size Available size for layout computation.
     * @param pos  Parent-relative position for layout computation.
     * @return true if the button was clicked during this frame.
     */
    bool operator()(const ImVec2& size, const ImVec2& pos);

    /**
     * @brief Access the base button color.
     *
     * Returns a mutable reference to the color used for the normal button state.
     * Modifications affect only this button instance.
     */
    ImVec4& color();

    /**
     * @brief Access the hover-state button color.
     *
     * Returns a mutable reference to the color used when the button is hovered.
     * Modifications affect only this button instance.
     */
    ImVec4& hover_color();

    /**
     * @brief Access the active-state button color.
     *
     * Returns a mutable reference to the color used while the button is active
     * (pressed).
     */
    ImVec4& active_color();

    /**
     * @brief Access the button text color.
     *
     * Returns a mutable reference to the color used to render the button label.
     */
    ImVec4& text_color();
};

} // namespace xyzlabs
