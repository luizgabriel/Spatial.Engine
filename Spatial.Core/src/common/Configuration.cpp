#include <spatial/common/Configuration.h>

namespace spatial
{

toml::node_view<const toml::node> findInNodeTree(const toml::table& table, const std::string_view name)
{
	auto argNames = split(name, '.');
	auto node = table[argNames[0]];

	for (std::size_t i = 1; i < argNames.size(); i++)
		node = node[argNames[i]];

	return node;
}

} // namespace spatial
