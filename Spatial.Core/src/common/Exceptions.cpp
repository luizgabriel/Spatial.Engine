#include <spatial/common/Exceptions.h>
#include <fmt/format.h>

namespace spatial
{

FileNotFoundError::FileNotFoundError(const std::filesystem::path& file)
	: Base(fmt::format("File not found: {}", file.generic_string()))
{
}

} // namespace spatial