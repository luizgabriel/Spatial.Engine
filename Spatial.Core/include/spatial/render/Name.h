#pragma once

#include <string>

namespace spatial
{

class Name
{
  private:
	std::string mValue;

  public:
	explicit Name(std::string value);

	const std::string& getValue() const noexcept;

	void setValue(const std::string& value) noexcept;
};

} // namespace spatial