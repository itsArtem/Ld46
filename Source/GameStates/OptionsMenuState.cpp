#include "OptionsMenuState.h"
#include "../Game.h"
#include "../Graphics/Gui/TextButton.h"
#include "GameplayState.h"

#include <SDL_render.h>

#include <string>

OptionsMenuState::OptionsMenuState(Game &game)
	: game{game},
	title{game.fontc.get(2), "Options", {255, 255, 100, 255}, true, game.getRenderer()}
{
	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	title.dstRect.y = 50;

	fullscreen.dstRect.x = game.getWindowSize().x - 600.0f;
	fullscreen.dstRect.y = 195;

	vSync.dstRect.x = game.getWindowSize().x - 600.0f;
	vSync.dstRect.y = 295;
	
	restartWarning.dstRect.x = vSync.dstRect.x + vSync.dstRect.w + 16.0f;
	restartWarning.dstRect.y = vSync.dstRect.y;

	setFullscreenText();
	setVSyncText();

	restartWarning.setText(game.fontc.get(1), "Requires restart", {255, 100, 100, 255}, false, game.getRenderer());

	canvas.add(std::make_unique<TextButton>(SDL_Point{200, 295}, game, TextButton::Properties{Text{game.fontc.get(1), "Back", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));

	canvas.add(std::make_unique<TextButton>(SDL_Point{game.getWindowSize().x - 600, 245}, game, TextButton::Properties{Text{game.fontc.get(1), "Toggle Fullscreen", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
	canvas.add(std::make_unique<TextButton>(SDL_Point{game.getWindowSize().x - 600, 345}, game, TextButton::Properties{Text{game.fontc.get(1), "Toggle VSync", {255, 255, 255, 255}, true, game.getRenderer()}, ButtonHoverEffect::enlarge}));
}

void OptionsMenuState::update() noexcept
{
	const int winX = game.getWindowSize().x;

	title.dstRect.x = game.getWindowSize().x / 2 - title.dstRect.w / 2;
	fullscreen.dstRect.x = winX - 600.0f;
	vSync.dstRect.x = winX - 600.0f;
	restartWarning.dstRect.x = vSync.dstRect.x + vSync.dstRect.w + 16.0f;
	
	for (std::size_t i = 1; i < canvas.getSize(); ++i)
		canvas.get(i).tf.x = winX - 600;
	
	canvas.update();

	if (canvas.get(0).isReleased())
	{
		game.config.save();
		game.gsm.pop();
		return;
	}

	if (canvas.get(1).isReleased())
	{
		game.config.fullscreen = !game.config.fullscreen;
		game.setFullscreen(game.config.fullscreen);
		setFullscreenText();
	}

	if (canvas.get(2).isReleased())
	{
		game.config.vSync = !game.config.vSync;
		setVSyncText();
	}
}

void OptionsMenuState::render() const noexcept
{
	SDL_Renderer *const rdr = game.getRenderer();

	title.render(rdr, Text::Shadow::none);
	fullscreen.render(rdr, Text::Shadow::none);
	vSync.render(rdr, Text::Shadow::none);
	restartWarning.render(rdr, Text::Shadow::none);
	
	canvas.render(rdr);
}

void OptionsMenuState::setFullscreenText() noexcept
{
	const std::string value{game.config.fullscreen ? "On" : "Off"};
	fullscreen.setText(game.fontc.get(1), "Fullscreen: " + value, {150, 150, 150, 255}, false, game.getRenderer());
}

void OptionsMenuState::setVSyncText() noexcept
{
	const std::string value{game.config.vSync ? "On" : "Off"};
	vSync.setText(game.fontc.get(1), "VSync: " + value, {150, 150, 150, 255}, false, game.getRenderer());
}
