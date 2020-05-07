#pragma once

#include "GameState.h"
#include "../Graphics/Gui/GuiCanvas.h"
#include "../Graphics/Text.h"
#include "../Game.h"

class OptionsMenuState final : public GameState
{
public:
	OptionsMenuState(Game &game);

	void update() noexcept override;
	void render() const noexcept override;

	void setFullscreenText() noexcept;
	void setVSyncText() noexcept;

private:
	Game &game;
	Text title, fullscreen, vSync, restartWarning;
	GuiCanvas canvas;
};