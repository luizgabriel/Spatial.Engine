#pragma once

#include "Settings.h"
#include <spatial/resources/AggregateFileSystem.h>
#include <spatial/resources/PhysicalFileSystem.h>
#include <utility>

namespace spatial::editor
{

SharedFileSystem createEditorFileSystem();

std::optional<SharedFileSystem> createProjectFileSystem(const editor::Settings& settings);

} // namespace spatial::editor