#pragma once

#include "PopupModal.h"
namespace spatial::ui
{

class NewSceneModal
{
  public:
	NewSceneModal();

	bool onConfirm();

  private:
	PopupModal mModal;
	bool mIsConfirmed{false};
};

}