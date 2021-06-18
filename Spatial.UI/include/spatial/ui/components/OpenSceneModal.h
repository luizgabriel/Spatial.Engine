#pragma once

#include <string>
#include <spatial/ui/components/PopupModal.h>

namespace spatial::ui
{

class OpenSceneModal
{
  public:
	OpenSceneModal(std::string& openPath);
	bool onConfirm();

  private:
	PopupModal mModal;
	bool mIsConfirmed;
};

} // namespace spatial::ui