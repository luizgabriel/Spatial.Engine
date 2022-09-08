#pragma once

#include <spatial/common/Math.h>

namespace spatial::ui
{

struct Icon
{
	math::vec2 pos;
	constexpr explicit Icon(math::vec2 pos) : pos{pos}
	{
	}

	[[nodiscard]] constexpr auto uv() const
	{
		const auto offset = math::vec2{1.0f / 8.0f};
		return math::vec4{pos * offset, (pos + math::vec2{1, 1}) * offset};
	}
};

struct Icons
{
	constexpr static Icon back = Icon{{0, 1}};
	constexpr static Icon folder = Icon{{0, 0}};
	constexpr static Icon unknownFile = Icon{{1, 0}};
	constexpr static Icon meshFile = Icon{{2, 0}};
	constexpr static Icon pngFile = Icon{{3, 0}};
	constexpr static Icon jpgFile = Icon{{4, 0}};
	constexpr static Icon exrFile = Icon{{5, 0}};
	constexpr static Icon ktxFile = Icon{{5, 1}};
	constexpr static Icon sceneFile = Icon{{6, 0}};
	constexpr static Icon jsFile = Icon{{7, 0}};
	constexpr static Icon logo = Icon{{0, 2}};
	constexpr static Icon picture = Icon{{3, 1}};
	constexpr static Icon bwPicture = Icon{{4, 2}};
	constexpr static Icon picker = Icon{{3, 2}};
	constexpr static Icon cubemap = Icon{{1, 1}};
	constexpr static Icon normalMap = Icon{{5, 2}};
	constexpr static Icon search = Icon{{0, 3}};
	Icons() = delete;
};

} // namespace spatial::ui