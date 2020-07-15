#pragma once

namespace spatial
{

template <typename BufferType>
using AssetReadAction = std::function<bool(std::string_view, BufferType)>;

using BasicBufferType = std::back_insert_iterator<std::vector<char>>;

}