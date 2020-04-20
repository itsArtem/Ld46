#include "MessageOutput.h"
#include "../../Game.h"

MessageOutput::MessageOutput(const Game &game) noexcept
	: game{game}
{
}

void MessageOutput::update() noexcept
{
	timer -= 1 * game.getDelta();
}

void MessageOutput::render() const noexcept
{
	if (timer <= 0.0f) return;
	SDL_Renderer *const rdr = game.getRenderer();

	for (float i = cellSize; i < msgSize.x; i += cellSize)
	{
		const SDL_FRect uedgeDstRect{pos.x + i, pos.y, cellSize, cellSize};
		SDL_RenderCopyF(rdr, bg, &edgeSrcRect, &uedgeDstRect);
		
		const SDL_FRect ledgeDstRect{pos.x + i, pos.y + msgSize.y, cellSize, cellSize};
		SDL_RenderCopyExF(rdr, bg, &edgeSrcRect, &ledgeDstRect, 0.0, nullptr, SDL_FLIP_VERTICAL);
	}

	const SDL_FRect uldstRect{pos.x, pos.y, cellSize, cellSize};
	SDL_RenderCopyF(rdr, bg, &cornerSrcRect, &uldstRect);
	
	const SDL_FRect urdstRect{pos.x + msgSize.x, pos.y, cellSize, cellSize};
	SDL_RenderCopyExF(rdr, bg, &cornerSrcRect, &urdstRect, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
	
	const SDL_FRect lldstRect{pos.x, pos.y + msgSize.y, cellSize, cellSize};
	SDL_RenderCopyExF(rdr, bg, &cornerSrcRect, &lldstRect, -90.0, nullptr, SDL_FLIP_NONE);
	
	const SDL_FRect lrdstRect{pos.x + msgSize.x, pos.y + msgSize.y, cellSize, cellSize};
	SDL_RenderCopyExF(rdr, bg, &cornerSrcRect, &lrdstRect, -90.0, nullptr, SDL_FLIP_VERTICAL);

	message.render(rdr, Text::Shadow::none);
}

void MessageOutput::showMessage(const std::string &msg, float time, SDL_FPoint pos)
{
	message.setText(game.fontc.get(0), msg, {255, 255, 255, 255}, true, game.getRenderer());
	
	this->pos.x = pos.x;
	this->pos.y = pos.y;

	message.dstRect.x = pos.x + cellSize / 2;
	message.dstRect.y = pos.y + cellSize / 2;

	msgSize.x = message.dstRect.w;
	msgSize.y = message.dstRect.h;

	timer = time;
}
