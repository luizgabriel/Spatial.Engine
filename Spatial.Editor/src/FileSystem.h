#pragma once

#include "Settings.h"
#include <spatial/resources/FilesSystem.h>

namespace spatial::editor
{

SharedFileSystem createDefaultFileSystem(const editor::Settings& settings);

} // namespace spatial::editor