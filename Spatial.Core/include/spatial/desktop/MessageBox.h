#pragma once

#include <string_view>

namespace spatial
{

enum class MessageBoxType
{
	Info,
	Warning,
	Error,
};

void showMessageBox(MessageBoxType type, std::string_view title, std::string_view message);

} // namespace spatial