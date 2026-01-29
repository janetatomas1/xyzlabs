#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace xyzlabs {

/**
 * @brief Abstract interface for a configurable setting or a group of settings.
 *
 * SettingInterface represents a node in a hierarchical settings tree.
 * A node may be:
 *  - a **leaf setting** (holding a concrete value), or
 *  - a **settings group** (containing child settings).
 *
 * Implementations are responsible for:
 *  - serializing/deserializing themselves to/from JSON,
 *  - presenting a UI representation,
 *  - supporting hierarchical access via child nodes.
 *
 * The interface is intentionally minimal and polymorphic, allowing
 * heterogeneous settings to coexist in the same tree.
 */
class SettingInterface {
protected:
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~SettingInterface() = default;

    /**
     * @brief Add a child setting to this node.
     *
     * This is primarily intended for settings groups.
     * Leaf settings typically ignore this call.
     *
     * @param key   Identifier under which the child is stored.
     * @param child Owning pointer to the child setting.
     * @return Pointer to the added child, or nullptr if unsupported.
     */
    virtual SettingInterface* add_child(
        const std::string &key,
        std::unique_ptr<SettingInterface> child
    ) { return nullptr; }

    /**
     * @brief Serialize this setting (and its children) to JSON.
     *
     * For groups, this should recursively serialize all children.
     * For leaf settings, this should return the stored value.
     *
     * @return JSON representation of the setting.
     */
    virtual json to_json() const = 0;

    /**
     * @brief Populate this setting from a JSON representation.
     *
     * For groups, this should dispatch JSON values to matching children.
     * For leaf settings, this should parse and store the value.
     *
     * @param j JSON object to deserialize from.
     */
    virtual void from_json(const json& j) = 0;

    /**
     * @brief Render the setting in a UI context.
     *
     * Implementations are expected to draw an interactive representation
     * (e.g., via ImGui or another GUI framework).
     *
     * @param label Display label for this setting.
     */
    virtual void show(const std::string &label) = 0;

    /**
     * @brief Create a deep copy of this setting.
     *
     * Required for duplicating settings trees without shared ownership.
     *
     * @return Owning pointer to the cloned setting.
     */
    virtual std::unique_ptr<SettingInterface> clone() const = 0;

    /**
     * @brief Retrieve a child setting by path.
     *
     * The path format is implementation-defined, but is typically
     * a delimiter-separated hierarchy (e.g., "graphics/shadows/quality").
     *
     * Leaf settings usually return nullptr.
     *
     * @param path Path identifying the child.
     * @return Pointer to the child setting, or nullptr if not found.
     */
    virtual SettingInterface* get_child(const std::string &path) { return nullptr; }
};

} // namespace xyzlabs
