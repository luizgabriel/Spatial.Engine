#include <spatial/common/StringHelpers.h>

namespace spatial
{

std::vector<std::string_view> split(const std::string_view str, const char separator)
{
	std::vector<std::string_view> arr{2};
	std::size_t start = 0, end = 0;

	for (size_t i = 0; i < str.length() && end != std::string_view::npos; i++)
	{
		end = std::string_view::npos;
		for (auto j = start; j < str.length(); j++)
		{
			if (str[j] == separator)
			{
				end = j;
				break;
			}
		}

		arr[i] = str.substr(start, end - start);
		start = end + 1;
	}

	return arr;
}

} // namespace spatial
