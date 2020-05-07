#include "ShopState.h"
#include "../Graphics/Gui/TextureButton.h"
#include "../World/World.h"
#include "../World/Ecs/Entity.h"
#include "../World/Ecs/Component.h"

#include <SDL_pixels.h>

ShopState::ShopState(const Game &game, GameStateManager &gsm, World &world) noexcept
	: game{game},
	gsm{gsm},
	world{world},
	title1{game.fontc.get(0), "Wood", {255, 255, 255, 255}, true, game.getRenderer()},
	desc1{game.fontc.get(0), "200 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost1{game.fontc.get(0), "30 Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title2{game.fontc.get(0), "Stone Bricks", {255, 255, 255, 255}, true, game.getRenderer()},
	desc2{game.fontc.get(0), "250 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost2{game.fontc.get(0), "40 Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title3{game.fontc.get(0), "Bricks", {255, 255, 255, 255}, true, game.getRenderer()},
	desc3{game.fontc.get(0), "300 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost3{game.fontc.get(0), "45 Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title4{game.fontc.get(0), "Iron", {255, 255, 255, 255}, true, game.getRenderer()},
	desc4{game.fontc.get(0), "350 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost4{game.fontc.get(0), "50 Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title5{game.fontc.get(0), "Obsidian", {255, 255, 255, 255}, true, game.getRenderer()},
	desc5{game.fontc.get(0), "450 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost5{game.fontc.get(0), "60 Points", {255, 255, 255, 255}, true, game.getRenderer()}
{
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{tf.x + tf.w + 2, tf.y + 6, 24, 24}, game, TextureButton::Visuals{ss, {0, 16, 8, 8}, {8, 16, 8, 8}, {16, 16, 8, 8}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{64, 64, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 16, 16, 16}, SDL_Rect{96, 16, 16, 16}, SDL_Rect{112, 16, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{64, 136, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 32, 16, 16}, SDL_Rect{96, 32, 16, 16}, SDL_Rect{112, 32, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{64, 208, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 48, 16, 16}, SDL_Rect{96, 48, 16, 16}, SDL_Rect{112, 48, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{64, 280, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 64, 16, 16}, SDL_Rect{96, 64, 16, 16}, SDL_Rect{112, 64, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{64, 352, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 80, 16, 16}, SDL_Rect{96, 80, 16, 16}, SDL_Rect{112, 80, 16, 16}}));
}

void ShopState::update() noexcept
{
	canvas.update();

	tf.w = game.getWindowSize().x - 96; 
	tf.h = game.getWindowSize().y - 96;

	canvas.get(0).tf.x = tf.w + 24;
	canvas.get(0).tf.y = tf.y + 16;

	if (canvas.get(0).isReleased())
	{
		exit();
		return;
	}

	title1.dstRect.x = 140.0f;
	title1.dstRect.y = 85.0f;

	desc1.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2 + 20;
	desc1.dstRect.y = 85.0f;
	
	cost1.dstRect.x = game.getWindowSize().x - 180.0f;
	cost1.dstRect.y = 85.0f;

	title2.dstRect.x = 140.0f;
	title2.dstRect.y = 85.0f + 72;

	desc2.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2 + 20;
	desc2.dstRect.y = 85.0f + 72;

	cost2.dstRect.x = game.getWindowSize().x - 180.0f;
	cost2.dstRect.y = 85.0f + 72;

	title3.dstRect.x = 140.0f;
	title3.dstRect.y = 85.0f + 72 * 2;

	desc3.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2 + 20;
	desc3.dstRect.y = 85.0f + 72 * 2;

	cost3.dstRect.x = game.getWindowSize().x - 180.0f;
	cost3.dstRect.y = 85.0f + 72 * 2;

	title4.dstRect.x = 140.0f;
	title4.dstRect.y = 85.0f + 72 * 3;

	desc4.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2 + 20;
	desc4.dstRect.y = 85.0f + 72 * 3;

	cost4.dstRect.x = game.getWindowSize().x - 180.0f;
	cost4.dstRect.y = 85.0f + 72 * 3;

	title5.dstRect.x = 140.0f;
	title5.dstRect.y = 85.0f + 72 * 4;

	desc5.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2 + 20;
	desc5.dstRect.y = 85.0f + 72 * 4;

	cost5.dstRect.x = game.getWindowSize().x - 180.0f;
	cost5.dstRect.y = 85.0f + 72 * 4;

	Entity *const player = world.em.getEntity(world.getPlayer());
	if (!player) return;
	PlayerComponent *const pc = player->getComponent<PlayerComponent>();
	if (!pc) return;
	
	const SDL_Colour selectedColour{255, 255, 100, 255};

	title1.colourMod = {255, 255, 255, 255};
	desc1.colourMod = {255, 255, 255, 255};
	cost1.colourMod = {255, 255, 255, 255};

	title2.colourMod = {255, 255, 255, 255};
	desc2.colourMod = {255, 255, 255, 255};
	cost2.colourMod = {255, 255, 255, 255};
	
	title3.colourMod = {255, 255, 255, 255};
	desc3.colourMod = {255, 255, 255, 255};
	cost3.colourMod = {255, 255, 255, 255};

	title4.colourMod = {255, 255, 255, 255};
	desc4.colourMod = {255, 255, 255, 255};
	cost4.colourMod = {255, 255, 255, 255};

	title5.colourMod = {255, 255, 255, 255};
	desc5.colourMod = {255, 255, 255, 255};
	cost5.colourMod = {255, 255, 255, 255};

	switch (pc->tileSelected)
	{
	case 3:
		title1.colourMod = selectedColour;
		desc1.colourMod = selectedColour;
		cost1.colourMod = selectedColour;
		break;
	case 6:
		title2.colourMod = selectedColour;
		desc2.colourMod = selectedColour;
		cost2.colourMod = selectedColour;
		break;
	case 7:
		title3.colourMod = selectedColour;
		desc3.colourMod = selectedColour;
		cost3.colourMod = selectedColour;
		break;
	case 8:
		title4.colourMod = selectedColour;
		desc4.colourMod = selectedColour;
		cost4.colourMod = selectedColour;
		break;
	case 9:
		title5.colourMod = selectedColour;
		desc5.colourMod = selectedColour;
		cost5.colourMod = selectedColour;
		break;
	}

	if (canvas.get(1).isReleased())
		pc->tileSelected = 3;

	if (canvas.get(2).isReleased())
		pc->tileSelected = 6;

	if (canvas.get(3).isReleased())
		pc->tileSelected = 7;

	if (canvas.get(4).isReleased())
		pc->tileSelected = 8;

	if (canvas.get(5).isReleased())
		pc->tileSelected = 9;
}

void ShopState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	const SDL_Rect dim{0, 0, game.getWindowSize().x, game.getWindowSize().y};
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 210);
	SDL_RenderFillRect(rdr, &dim);

	for (int i = cellSize; i < tf.w; i += cellSize)
	{
		const SDL_Rect uedgeDstRect{tf.x + i, tf.y, cellSize, cellSize};
		SDL_RenderCopy(rdr, ss, &edgeSrcRect, &uedgeDstRect);

		const SDL_Rect ledgeDstRect{tf.x + i, tf.y + tf.h, cellSize, cellSize};
		SDL_RenderCopyEx(rdr, ss, &edgeSrcRect, &ledgeDstRect, 0.0, nullptr, SDL_FLIP_VERTICAL);
	}

	for (int i = cellSize; i < tf.h; i += cellSize)
	{
		const SDL_Rect ledgeDstRect{tf.x, tf.y + i, cellSize, cellSize};
		SDL_RenderCopyEx(rdr, ss, &edgeSrcRect, &ledgeDstRect, -90.0, nullptr, SDL_FLIP_NONE);

		const SDL_Rect redgeDstRect{tf.x + tf.w, tf.y + i, cellSize, cellSize};
		SDL_RenderCopyEx(rdr, ss, &edgeSrcRect, &redgeDstRect, 90.0, nullptr, SDL_FLIP_NONE);
	}

	const SDL_Rect uldstRect{tf.x, tf.y, cellSize, cellSize};
	SDL_RenderCopy(rdr, ss, &cornerSrcRect, &uldstRect);

	const SDL_Rect urdstRect{tf.x + tf.w, tf.y, cellSize, cellSize};
	SDL_RenderCopyEx(rdr, ss, &cornerSrcRect, &urdstRect, 0.0, nullptr, SDL_FLIP_HORIZONTAL);

	const SDL_Rect lldstRect{tf.x, tf.y + tf.h, cellSize, cellSize};
	SDL_RenderCopyEx(rdr, ss, &cornerSrcRect, &lldstRect, -90.0, nullptr, SDL_FLIP_NONE);

	const SDL_Rect lrdstRect{tf.x + tf.w, tf.y + tf.h, cellSize, cellSize};
	SDL_RenderCopyEx(rdr, ss, &cornerSrcRect, &lrdstRect, -90.0, nullptr, SDL_FLIP_VERTICAL);

	const SDL_Rect centreDstRect{tf.x + cellSize, tf.y + cellSize, tf.w - cellSize, tf.h - cellSize};
	SDL_RenderCopy(rdr, ss, &centreSrcRect, &centreDstRect);

	canvas.render(rdr);

	title1.render(rdr, Text::Shadow::right);
	desc1.render(rdr, Text::Shadow::right);
	cost1.render(rdr, Text::Shadow::right);

	title2.render(rdr, Text::Shadow::right);
	desc2.render(rdr, Text::Shadow::right);
	cost2.render(rdr, Text::Shadow::right);

	title3.render(rdr, Text::Shadow::right);
	desc3.render(rdr, Text::Shadow::right);
	cost3.render(rdr, Text::Shadow::right);

	title4.render(rdr, Text::Shadow::right);
	desc4.render(rdr, Text::Shadow::right);
	cost4.render(rdr, Text::Shadow::right);

	title5.render(rdr, Text::Shadow::right);
	desc5.render(rdr, Text::Shadow::right);
	cost5.render(rdr, Text::Shadow::right);
}

void ShopState::exit() const noexcept
{
	gsm.renderPrevious(false);
	gsm.pop();
}
