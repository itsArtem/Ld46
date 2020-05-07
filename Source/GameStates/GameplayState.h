#pragma once

#include "GameState.h"
#include "../World/World.h"
#include "../Graphics/Hud/HudManager.h"
#include "../Graphics/Gui/GuiCanvas.h"

class Game;

class GameplayState final : public GameState
{
public:
	explicit GameplayState(Game &game) noexcept;

	void update() noexcept override;
	void render() const noexcept override;

private:
	Game &game;
	HudManager hudm;
	GuiCanvas canvas;

	World world;

	bool showedMove = false;
	bool showedShop = false;
	bool showedBuild = false;
	bool showedRemove = false;
	bool showedPoints = false;
	bool showedFinal = false;
	float tipTimer = 2.0f;
};