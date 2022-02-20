#pragma once

#include <filesystem>
#include <functional>
#include <spatial/resources/Resource.h>
#include <spatial/resources/ResourceError.h>
#include <type_traits>

namespace spatial
{

template <typename T, typename Result, typename Res>
using is_raw_resource_finder = std::is_same<Result, std::result_of_t<T(const Res&)>>;

template <typename T, ResourceType type, typename Result>
using is_resource_finder = is_raw_resource_finder<T, Result, Resource<type>>;

template <typename T, ResourceType type, typename Result>
constexpr auto is_resource_finder_v = is_resource_finder<T, type, Result>::value;

template <ResourceType type, typename Result>
using ResourceFinder = std::function<Result(const Resource<type>&)>;

} // namespace spatial