#pragma once

#include <spatial/common/Math.h>
namespace spatial::ui
{

struct Icon
{
	math::float4 uv;
	constexpr explicit Icon(const math::float4& uv) : uv(uv)
	{
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

constexpr math::float4 toIconUV(const math::float2& pos)
{
	const auto offset = math::float2{0.1, 0.12};
	return math::float4{pos * offset, (pos + math::float2{1, 1}) * offset} ;
}

constexpr auto gIcons = Icons{
	Icon{toIconUV({0, 1})},
	Icon{toIconUV({0, 0})},
	Icon{toIconUV({1, 0})},
	Icon{toIconUV({2, 0})},
	Icon{toIconUV({3, 0})},
	Icon{toIconUV({4, 0})},
	Icon{toIconUV({5, 0})},
	Icon{toIconUV({6, 0})},
	Icon{toIconUV({7, 0})},
};

} // namespace spatial::ui