#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace spatial::ecs
{

struct Name
{
	constexpr static auto typeName = "name";

	std::string name;

	[[nodiscard]] const char* c_str() const
	{
		return name.c_str();
	}
};

struct UUID
{
	constexpr static auto typeName = "uuid";

	boost::uuids::uuid value;

	UUID() : value{}
	{
	}

	explicit UUID(boost::uuids::uuid value) : value{std::move(value)}
	{
	}

	static UUID generate()
	{
		static auto generator = boost::uuids::random_generator{};
		return UUID{generator()};
	}

	bool operator==(const UUID& rhs) const
	{
		return value == rhs.value;
	}

	bool operator!=(const UUID& rhs) const
	{
		return !(rhs == *this);
	}

	bool operator<(const UUID& rhs) const
	{
		return value < rhs.value;
	}

	bool operator>(const UUID& rhs) const
	{
		return rhs < *this;
	}

	bool operator<=(const UUID& rhs) const
	{
		return !(rhs < *this);
	}

	bool operator>=(const UUID& rhs) const
	{
		return !(*this < rhs);
	}

	[[nodiscard]] std::string toString() const
	{
		return boost::uuids::to_string(value);
	}

	static UUID fromString(const std::string_view value)
	{
		return UUID{boost::lexical_cast<boost::uuids::uuid>(value)};
	}
};

} // namespace spatial::ecs

template <>
struct std::hash<spatial::ecs::UUID>
{
	std::size_t operator()(const spatial::ecs::UUID& id) const noexcept
	{
		return hash_value(id.value);
	}
};