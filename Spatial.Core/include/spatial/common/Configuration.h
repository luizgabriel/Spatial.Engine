#pragma once

#include <spatial/common/StringHelpers.h>
#include <toml++/toml.h>
#include <optional>

namespace spatial {

toml::node_view<const toml::node> findInNodeTree(const toml::table &table, std::string_view name) noexcept;

class Configuration final
{
private:
  toml::table mTable;

public:
  explicit Configuration(toml::table table) : mTable{ std::move(table) } {}

  template<typename ValueType> std::optional<ValueType> get(const std::string_view name) const
  {
    auto node = findInNodeTree(mTable, name);
    return node.value<ValueType>();
  }

  template<typename ValueType> ValueType get(const std::string_view name, ValueType &&defaultValue) const
  {
    return get(name).value_or
  }
};
}// namespace spatial
