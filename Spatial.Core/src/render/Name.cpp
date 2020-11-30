#include <spatial/render/Name.h>

namespace spatial
{

Name::Name(std::string value) : mValue(std::move(value))
{
}

const std::string& Name::getValue() const noexcept
{
	return mValue;
}

void Name::setValue(const std::string& value) noexcept
{
	mValue = value;
}

} // namespace spatial