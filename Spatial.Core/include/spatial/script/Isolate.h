#pragma once

#include <v8-isolate.h>

namespace spatial::script
{

class Isolate
{
  public:
	explicit Isolate(const v8::Isolate::CreateParams& params);
	explicit Isolate(v8::Isolate* isolate);

	~Isolate();

  private:
	v8::Isolate* mIsolate;
};


}