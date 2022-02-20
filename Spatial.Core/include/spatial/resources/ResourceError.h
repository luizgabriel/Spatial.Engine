#pragma once

namespace spatial
{

enum class ResourceError
{
	UnauthorizedPath,
	NotFound,
	NotAFile,
	OpenFailed,
	ParseError,
	InvalidExtension
};

} // namespace spatial