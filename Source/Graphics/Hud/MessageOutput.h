#pragma once

#include "../Text.h"
#include "../../Game.h"

#include <SDL_render.h>
#include <SDL_rect.h>

class MessageOutput final
{
public:
	explicit MessageOutput(const Game &game) noexcept;

	void update() noexcept;
	void render() const noexcept;

	void showMessage(const std::string &msg, float time, SDL_FPoint pos);

private:
	const Game &game;
	Text message;

	SDL_Texture *const bg = game.texc.get(2);
	const SDL_Rect cornerSrcRect{0, 0, 16, 16};
	const SDL_Rect edgeSrcRect{16, 0, 16, 16};

	SDL_FPoint pos{0.0f, 0.0f};
	const float cellSize = 32.0f;
	SDL_FPoint msgSize{0.0f, 0.0f};

	float timer = 0.0f;
};