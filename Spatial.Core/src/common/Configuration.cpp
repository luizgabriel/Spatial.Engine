#include <spatial/common/Configuration.h>

namespace spatial
{

toml::node_view<const toml::node> findInNodeTree(const toml::table& table, const std::string_view name)
{
	auto argTree = split(name, '.');
	auto node = table[argTree[0]];

	for (std::size_t i = 1; i < argTree.size(); i++)
		node = node[argTree[i]];

	return node;
}

} // namespace spatial
