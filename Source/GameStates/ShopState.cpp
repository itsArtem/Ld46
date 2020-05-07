#include "ShopState.h"
#include "../Graphics/Gui/TextureButton.h"
#include "../World/World.h"
#include "../World/Ecs/Entity.h"
#include "../World/Ecs/Component.h"

#include <SDL_pixels.h>

#include <cstddef>
#include <cstdint>

ShopState::ShopState(const Game &game, GameStateManager &gsm, World &world) noexcept
	: game{game},
	gsm{gsm},
	world{world},
	title0{game.fontc.get(2), "Shop", {255, 255, 100, 255}, true, game.getRenderer()},
	title1{game.fontc.get(0), "Wood", {255, 255, 255, 255}, true, game.getRenderer()},
	desc1{game.fontc.get(0), "150 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost1{game.fontc.get(0), std::to_string(world.getTileCost(3)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title2{game.fontc.get(0), "Stone Bricks", {255, 255, 255, 255}, true, game.getRenderer()},
	desc2{game.fontc.get(0), "200 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost2{game.fontc.get(0), std::to_string(world.getTileCost(6)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title3{game.fontc.get(0), "Bricks", {255, 255, 255, 255}, true, game.getRenderer()},
	desc3{game.fontc.get(0), "250 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost3{game.fontc.get(0), std::to_string(world.getTileCost(7)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title4{game.fontc.get(0), "Metal", {255, 255, 255, 255}, true, game.getRenderer()},
	desc4{game.fontc.get(0), "250 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost4{game.fontc.get(0), std::to_string(world.getTileCost(10)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title5{game.fontc.get(0), "Iron", {255, 255, 255, 255}, true, game.getRenderer()},
	desc5{game.fontc.get(0), "300 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost5{game.fontc.get(0), std::to_string(world.getTileCost(8)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()},
	title6{game.fontc.get(0), "Obsidian", {255, 255, 255, 255}, true, game.getRenderer()},
	desc6{game.fontc.get(0), "450 HP", {255, 255, 255, 255}, true, game.getRenderer()},
	cost6{game.fontc.get(0), std::to_string(world.getTileCost(9)) + " Points", {255, 255, 255, 255}, true, game.getRenderer()}
{
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 24, 24}, game, TextureButton::Visuals{ss, {0, 16, 8, 8}, {8, 16, 8, 8}, {16, 16, 8, 8}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 16, 16, 16}, SDL_Rect{96, 16, 16, 16}, SDL_Rect{112, 16, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 32, 16, 16}, SDL_Rect{96, 32, 16, 16}, SDL_Rect{112, 32, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 48, 16, 16}, SDL_Rect{96, 48, 16, 16}, SDL_Rect{112, 48, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 96, 16, 16}, SDL_Rect{96, 96, 16, 16}, SDL_Rect{112, 96, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 64, 16, 16}, SDL_Rect{96, 64, 16, 16}, SDL_Rect{112, 64, 16, 16}}));
	canvas.add(std::make_unique<TextureButton>(SDL_Rect{0, 0, 64, 64}, game, TextureButton::Visuals{game.texc.get(2), SDL_Rect{80, 80, 16, 16}, SDL_Rect{96, 80, 16, 16}, SDL_Rect{112, 80, 16, 16}}));
}

void ShopState::update() noexcept
{
	canvas.update();
	
	tf.x = game.getWindowSize().x / 2 - tf.w / 2 - 16;
	tf.y = game.getWindowSize().y / 2 - tf.h / 2 - 16;

	canvas.get(0).tf.x = tf.x + tf.w - 7;
	canvas.get(0).tf.y = tf.y + 14;

	if (canvas.get(0).isReleased())
	{
		exit();
		return;
	}
	
	for (std::size_t i = 1; i < canvas.getSize(); ++i)
	{
		canvas.get(i).tf.x = tf.x + 48;
		canvas.get(i).tf.y = tf.y + 95 + 72 * (static_cast<int>(i) - 1);
	}
	
	const float titlePos = (game.getWindowSize().x / 2 - tf.w / 2) + 140.0f;

	title0.dstRect.x = game.getWindowSize().x / 2 - title0.dstRect.w / 2;
	title0.dstRect.y = tf.y + 24.0f;

	title1.dstRect.x = titlePos;
	title1.dstRect.y = tf.y + 115.0f;

	desc1.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc1.dstRect.y = tf.y + 115.0f;
	
	cost1.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost1.dstRect.y = tf.y + 115.0f;

	title2.dstRect.x = titlePos;
	title2.dstRect.y = tf.y + 115.0f + 72.0f;

	desc2.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc2.dstRect.y = tf.y + 115.0f + 72.0f;

	cost2.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost2.dstRect.y = tf.y + 115.0f + 72.0f;

	title3.dstRect.x = titlePos;
	title3.dstRect.y = tf.y + 115.0f + 72.0f * 2;

	desc3.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc3.dstRect.y = tf.y + 115.0f + 72.0f * 2;

	cost3.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost3.dstRect.y = tf.y + 115.0f + 72.0f * 2;

	title4.dstRect.x = titlePos;
	title4.dstRect.y = tf.y + 115.0f + 72.0f * 3;

	desc4.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc4.dstRect.y = tf.y + 115.0f + 72.0f * 3;

	cost4.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost4.dstRect.y = tf.y + 115.0f + 72.0f * 3;

	title5.dstRect.x = titlePos;
	title5.dstRect.y = tf.y + 115.0f + 72.0f * 4;

	desc5.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc5.dstRect.y = tf.y + 115.0f + 72.0f * 4;

	cost5.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost5.dstRect.y = tf.y + 115.0f + 72.0f * 4;

	title6.dstRect.x = titlePos;
	title6.dstRect.y = tf.y + 115.0f + 72.0f * 5;

	desc6.dstRect.x = game.getWindowSize().x / 2 - desc1.dstRect.w / 2;
	desc6.dstRect.y = tf.y + 115.0f + 72.0f * 5;

	cost6.dstRect.x = (game.getWindowSize().x / 2 - tf.w / 2) + tf.w - 180.0f;
	cost6.dstRect.y = tf.y + 115.0f + 72.0f * 5;

	Entity *const player = world.em.getEntity(world.getPlayer());
	if (!player) return;
	PlayerComponent *const pc = player->getComponent<PlayerComponent>();
	if (!pc) return;
	
	const SDL_Colour selectedColour{255, 255, static_cast<std::uint8_t>(blue), 255};
	blue += inc * game.getDelta();
	if (blue <= 0.0f || blue >= 255.0f)
	{
		if (blue < 0.0f) blue = 0.0f;
		if (blue > 255.0f) blue = 255.0f;

		inc = -inc;
	}

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

	title6.colourMod = {255, 255, 255, 255};
	desc6.colourMod = {255, 255, 255, 255};
	cost6.colourMod = {255, 255, 255, 255};

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
		title5.colourMod = selectedColour;
		desc5.colourMod = selectedColour;
		cost5.colourMod = selectedColour;
		break;
	case 9:
		title6.colourMod = selectedColour;
		desc6.colourMod = selectedColour;
		cost6.colourMod = selectedColour;
		break;
	case 10:
		title4.colourMod = selectedColour;
		desc4.colourMod = selectedColour;
		cost4.colourMod = selectedColour;
		break;
	}

	if (canvas.get(1).isReleased())
		pc->tileSelected = 3;

	if (canvas.get(2).isReleased())
		pc->tileSelected = 6;

	if (canvas.get(3).isReleased())
		pc->tileSelected = 7;

	if (canvas.get(4).isReleased())
		pc->tileSelected = 10;

	if (canvas.get(5).isReleased())
		pc->tileSelected = 8;

	if (canvas.get(6).isReleased())
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

	const SDL_Rect barDstRect{tf.x + tf.w / 4 + 8, tf.y + 72, tf.w / 2, 32};
	SDL_RenderCopy(rdr, ss, &barSrcRect, &barDstRect);

	canvas.render(rdr);
	title0.render(rdr, Text::Shadow::right);

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

	title6.render(rdr, Text::Shadow::right);
	desc6.render(rdr, Text::Shadow::right);
	cost6.render(rdr, Text::Shadow::right);
}

void ShopState::exit() const noexcept
{
	gsm.renderPrevious(false);
	gsm.pop();
}
