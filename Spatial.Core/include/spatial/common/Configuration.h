#pragma once

#include <spatial/common/StringHelpers.h>
#include <toml++/toml.h>

namespace spatial
{

toml::node_view<const toml::node> findInNodeTree(const toml::table& table, std::string_view name);

template <typename DataType>
DataType parseNodeValue(const toml::node_view<const toml::node> node)
{
	if constexpr (std::is_same_v<std::string, DataType>)
	{
		auto nodeView = node.as_string();
		return nodeView ? nodeView->get() : DataType{};
	}

	else if constexpr (std::is_floating_point_v<DataType>)
	{
		auto nodeView = node.as_floating_point();
		return nodeView ? static_cast<DataType>(nodeView->get()) : DataType{};
	}

	else if constexpr (std::is_integral_v<DataType>)
	{
		auto nodeView = node.as_integer();
		return nodeView ? static_cast<DataType>(nodeView->get()) : DataType{};
	}

	else
	{
		return DataType{};
	}
}

class Configuration final
{
private:
	toml::parse_result m_table;

public:
	explicit Configuration(toml::parse_result table) : m_table{std::move(table)} {}

	template <typename DataType>
	DataType get(const std::string_view name) const
	{
		auto node = findInNodeTree(m_table, name);
		return parseNodeValue<DataType>(node);
	}
};
} // namespace spatial
