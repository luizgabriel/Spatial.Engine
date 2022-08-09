#pragma once

#include <v8-script.h>

namespace spatial::script
{

class ScriptSourceStream : public v8::ScriptCompiler::ExternalSourceStream
{
  public:
	explicit ScriptSourceStream(std::unique_ptr<std::istream>&& stream);

  private:
	size_t GetMoreData(const uint8_t** src) override;

  private:
	std::unique_ptr<std::istream> mStream;
};

} // namespace spatial::script