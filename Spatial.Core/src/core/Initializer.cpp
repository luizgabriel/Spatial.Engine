#include <spatial/core/Initializer.h>
#include <spatial/desktop/MessageBox.h>
#include <stdexcept>

namespace spatial
{

int tryRun(const std::function<int(void)>& action)
{
	try
	{
		return action();
	}
	catch (const std::runtime_error& e)
	{
		showMessageBox(MessageBoxType::Error, "[Spatial Engine] Something went wrong", e.what());
		return -1;
	}
}


}