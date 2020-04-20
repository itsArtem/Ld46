#pragma once

#include "../Graphics/Animation.h"

class GameState
{
public:
	GameState(const GameState &gameState) noexcept = default;
	GameState(GameState &&gameState) noexcept = default;

	virtual ~GameState() = default;

	GameState &operator =(const GameState &gameState) noexcept = default;
	GameState &operator =(GameState && gameState) noexcept = default;

	virtual void update() noexcept = 0;
	virtual void render() const noexcept = 0;

protected:
	GameState() noexcept = default;
};