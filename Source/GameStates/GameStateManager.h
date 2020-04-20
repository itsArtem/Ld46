#pragma once

#include "GameState.h"

#include <vector>
#include <memory>

class GameStateManager final
{
public:
	void add(std::unique_ptr<GameState> gameState) noexcept;
	void pop() noexcept;
	void remove(std::size_t index) noexcept;
	void clear() noexcept;

	void update() noexcept;
	void render() const noexcept;
	void renderPrevious(bool renderPrevious) noexcept;
	
	[[nodiscard]] std::size_t getCount() const noexcept;

private:
	std::vector<std::unique_ptr<GameState>> gameStates;
	int previousIndex = -1;
};