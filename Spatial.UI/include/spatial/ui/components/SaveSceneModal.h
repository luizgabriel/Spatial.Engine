#pragma once

#include <spatial/ui/components/PopupModal.h>
#include <string>

namespace spatial::ui
{

class SaveSceneModal
{
  public:
	SaveSceneModal(std::string& savePath);

	bool onConfirm();

  private:
	PopupModal mModal;
	bool mConfirmed;
};

}
