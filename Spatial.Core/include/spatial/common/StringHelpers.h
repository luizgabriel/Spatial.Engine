#pragma once

#include <string_view>
#include <vector>

namespace spatial
{

namespace detail
{

template <typename H>
struct StringHashingValues;

template <>
struct StringHashingValues<uint32_t>
{
	static constexpr uint32_t basis = 2166136261UL;
	static constexpr uint32_t prime = 16777619UL;
};

template <>
struct StringHashingValues<uint64_t>
{
	static constexpr uint64_t basis = 14695981039346656037ULL;
	static constexpr uint64_t prime = 1099511628211ULL;
};

} // namespace detail

template <typename H, typename SV = std::string_view, typename C = typename SV::value_type>
constexpr H hash(const SV c, std::size_t idx = 0, H value = detail::StringHashingValues<H>::basis)
{
	return (c[idx] == C{})
			   ? value
			   : hash(c, idx + 1, static_cast<H>(value ^ static_cast<H>(c[idx])) * detail::StringHashingValues<H>::prime);
}

constexpr uint32_t operator"" _sh32(const char* str, std::size_t len)
{
	return hash<uint32_t>({str, len});
}

constexpr uint64_t operator"" _sh64(const char* str, std::size_t len)
{
	return hash<uint64_t>({str, len});
}

std::vector<std::string_view> split(std::string_view str, char separator);

} // namespace spatial
