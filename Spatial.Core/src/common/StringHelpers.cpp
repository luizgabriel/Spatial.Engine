#include <spatial/common/StringHelpers.h>

namespace spatial
{

std::vector<std::string_view> split(std::string_view value, char separator)
{
	std::vector<std::string_view> output;
	size_t first = 0;

	while (first < value.size())
	{
		const auto second = value.find_first_of(separator, first);

		if (first != second)
			output.emplace_back(value.substr(first, second - first));

		if (second == std::string_view::npos)
			break;

		first = second + 1;
	}

	return output;
}

} // namespace spatial
