#pragma once

#include "Graphics/TextureCache.h"
#include "Graphics/FontCache.h"
#include "AudioCache.h"
#include "GameStates/GameStateManager.h"

#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_rect.h>

#include <thread>

class Game final
{
public:
	bool running = false;

	TextureCache texc;
	FontCache fontc;
	AudioCache audioc;

	GameStateManager gsm;

	Game();

	Game(const Game &game) = delete;
	Game(Game &&game) noexcept;

	~Game();

	Game &operator =(const Game &game) = delete;
	Game &operator =(Game &&game) noexcept;

	void run() noexcept;
	void setFullscreen(bool fullscreen) noexcept;
	[[nodiscard]] SDL_Point getWindowSize() const noexcept;

	void toMainMenu() noexcept;

	[[nodiscard]] SDL_Renderer *getRenderer() const noexcept { return rdr; }
	[[nodiscard]] float getDelta() const noexcept { return delta; }

private:
	SDL_Window *window = SDL_CreateWindow("Defend the Loot!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 680, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *rdr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	float delta = 0.0f;

	void update() noexcept;
	void render() const noexcept;

	void loopMusic() noexcept;

	void moveData(Game &game) noexcept;
	void destroyGraphics() noexcept;
};