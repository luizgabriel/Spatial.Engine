#pragma once

#include <stdexcept>
#include <filesystem>

namespace spatial
{

class FileNotFoundError : public std::runtime_error
{
public:
	using Base = std::runtime_error;

	explicit FileNotFoundError(const std::filesystem::path& file);
};

} // namespace spatial