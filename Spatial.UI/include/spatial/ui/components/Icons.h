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
	Icon back;
	Icon folder;
	Icon unknownFile;
	Icon meshFile;
	Icon pngFile;
	Icon jpgFile;
	Icon exrFile;
	Icon sceneFile;
	Icon jsFile;

	constexpr Icons(const Icon& back, const Icon& folder, const Icon& unknownFile, const Icon& meshFile, const Icon& pngFile,
		  const Icon& jpgFile, const Icon& exrFile, const Icon& sceneFile, const Icon& jsFile)
		: back(back),
		  folder(folder),
		  unknownFile(unknownFile),
		  meshFile(meshFile),
		  pngFile(pngFile),
		  jpgFile(jpgFile),
		  exrFile(exrFile),
		  sceneFile(sceneFile),
		  jsFile(jsFile)
	{
	}
};

constexpr auto gIcons = Icons{
	Icon{{0, 1}},
	Icon{{0, 0}},
	Icon{{1, 0}},
	Icon{{2, 0}},
	Icon{{3, 0}},
	Icon{{4, 0}},
	Icon{{5, 0}},
	Icon{{6, 0}},
	Icon{{7, 0}},
};

} // namespace spatial::ui