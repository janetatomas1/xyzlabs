#pragma once

#include <boost/unordered_map.hpp>

#include "xyzlabs/settings/settinginterface.hpp"
#include "xyzlabs/settings/settingbase.hpp"

namespace xyzlabs {

/**
 * @brief Hierarchical container for settings.
 *
 * SettingsGroup implements a composite node in the settings tree.
 * It owns and manages child SettingInterface instances, which may be
 * either leaf settings or nested SettingsGroup objects.
 *
 * Children are addressed using dot-separated paths, for example:
 * @code
 * graphics.shadows.quality
 * @endcode
 *
 * The group supports:
 *  - recursive insertion and lookup by path,
 *  - JSON serialization/deserialization,
 *  - hierarchical UI rendering using ImGui,
 *  - deep cloning of the entire subtree.
 */
class SettingsGroup : public SettingInterface {
    /**
     * @brief Map of direct child settings.
     *
     * Each entry represents one node in the hierarchy. Ownership of
     * all children is exclusive.
     */
    boost::unordered_map<std::string, std::unique_ptr<SettingInterface>> settings_;

public:
    /**
     * @brief Add a setting at the given hierarchical path.
     *
     * Intermediate SettingsGroup nodes are created automatically
     * if they do not already exist.
     *
     * This is a convenience wrapper around add_child().
     *
     * @param path    Dot-separated path identifying the setting.
     * @param ptr     Owning pointer to the setting.
     * @return Pointer to the inserted setting, or nullptr on failure.
     */
    SettingInterface* add_setting(
        const std::string &path,
        std::unique_ptr<SettingInterface> ptr
    );

    /**
     * @brief Retrieve a setting by hierarchical path.
     *
     * This is a convenience wrapper around get_child().
     *
     * @param path Dot-separated path identifying the setting.
     * @return Pointer to the setting, or nullptr if not found.
     */
    SettingInterface* get(const std::string &path);

    /**
     * @brief Add a child setting using a hierarchical path.
     *
     * If the path contains multiple components, intermediate
     * SettingsGroup nodes are created as needed.
     *
     * If a setting already exists at the given path, it is rewritten
     *
     * @param path  Dot-separated insertion path.
     * @param child Owning pointer to the child setting.
     * @return Pointer to the inserted setting, or nullptr on failure.
     */
    SettingInterface* add_child(
        const std::string &path,
        std::unique_ptr<SettingInterface> child
    ) override;

    /**
     * @brief Retrieve a child setting using a hierarchical path.
     *
     * Path resolution is performed recursively. Each component
     * selects a child in the corresponding SettingsGroup.
     *
     * @param path Dot-separated lookup path.
     * @return Pointer to the setting, or nullptr if not found.
     */
    SettingInterface* get_child(const std::string &path) override;

    /**
     * @brief Serialize this settings group to JSON.
     *
     * Each child is serialized under its key. Nested groups
     * produce nested JSON objects.
     *
     * @return JSON object representing the group.
     */
    json to_json() const override;

    /**
     * @brief Populate this settings group from JSON.
     *
     * Only existing children are deserialized. Unknown JSON keys
     * are ignored, allowing forward/backward compatibility.
     *
     * @param j JSON object to deserialize from.
     */
    void from_json(const json& j) override;

    /**
     * @brief Render the settings group using ImGui.
     *
     * Nested SettingsGroup objects are rendered as tree nodes.
     * Leaf settings (derived from SettingBase) are rendered in
     * a two-column table with labels and controls.
     *
     * @param label Display label for the group.
     */
    void show(const std::string &label) override;

    /**
     * @brief Create a deep copy of this settings group.
     *
     * All children are recursively cloned, producing a fully
     * independent settings subtree.
     *
     * @return Owning pointer to the cloned group.
     */
    std::unique_ptr<SettingInterface> clone() const override;

    /**
     * @brief Construct an empty settings group.
     */
    SettingsGroup() = default;
};

} // namespace xyzlabs
