#include "GameStateManager.h"

void GameStateManager::add(std::unique_ptr<GameState> gameState) noexcept
{
	gameStates.emplace_back(std::move(gameState));
}

void GameStateManager::pop() noexcept
{
	gameStates.pop_back();
}

void GameStateManager::remove(std::size_t index) noexcept
{
	gameStates.erase(gameStates.begin() + index);
}

void GameStateManager::clear() noexcept
{
	gameStates.clear();
}

void GameStateManager::update() noexcept
{
	gameStates.back()->update();
}

void GameStateManager::render() const noexcept
{
	if (previousIndex > -1)
		gameStates[previousIndex]->render();

	gameStates.back()->render();
}

void GameStateManager::renderPrevious(bool renderPrevious) noexcept
{
	if (renderPrevious)
		previousIndex = gameStates.size() - 2;
	else
		previousIndex = -1;
}

std::size_t GameStateManager::getCount() const noexcept
{
	return gameStates.size();
}
