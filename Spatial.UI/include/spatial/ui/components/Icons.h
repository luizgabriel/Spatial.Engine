#pragma once

#include <spatial/common/Math.h>

namespace spatial::ui
{

struct Icon
{
	math::float2 pos;
	constexpr explicit Icon(math::float2 pos) : pos(pos)
	{
	}

	[[nodiscard]] constexpr auto uv() const
	{
		const auto offset = math::float2{0.1, 0.12};
		return math::float4{pos * offset, (pos + math::float2{1, 1}) * offset};
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
	constexpr static Icon ktxFile= Icon{{5, 1}};
	constexpr static Icon sceneFile = Icon{{6, 0}};
	constexpr static Icon jsFile = Icon{{7, 0}};
	Icons() = delete;
};

} // namespace spatial::ui