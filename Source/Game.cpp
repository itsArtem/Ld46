#include "Game.h"
#include "Debug.h"
#include "GameStates/MainMenuState.h"

#include <SDL_events.h>
#include <SDL_error.h>
#include <SDL_log.h>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdexcept>
#include <utility>
#include <chrono>
#include <memory>
#include <thread>
#include <cstdint>

Game::Game()
{
	if (!window || !rdr)
	{
		if (window) SDL_DestroyWindow(window);
		else if (rdr) SDL_DestroyRenderer(rdr);

		throw std::runtime_error{SDL_GetError()};
	}

	SDL_SetWindowMinimumSize(window, 1100, 680);
	SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);

	SDL_Surface *icon = IMG_Load("Resources/Textures/Icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	texc.load("Resources/Textures/TileSheet.png", rdr);
	texc.load("Resources/Textures/EntitySheet.png", rdr);
	texc.load("Resources/Textures/UiSheet.png", rdr);

	fontc.open("Resources/Fonts/Font.ttf", 26);
	fontc.open("Resources/Fonts/Font.ttf", 32);
	fontc.open("Resources/Fonts/Font.ttf", 48);

	audioc.loadMusic("Resources/Audio/Music/Track1.mid");
	audioc.loadMusic("Resources/Audio/Music/Track2.mid");
	audioc.loadMusic("Resources/Audio/Music/Track3.mid");
	audioc.loadMusic("Resources/Audio/Music/Track4.mid");

	audioc.loadChunk("Resources/Audio/Chunks/Attack.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Break.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hit.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hostile.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hurt.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Place.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Remove.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Select.wav");

	gsm.add(std::make_unique<MainMenuState>(*this));
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
}

Game::Game(Game &&game) noexcept
{
	moveData(game);
}

Game::~Game()
{
	destroyGraphics();
}

Game &Game::operator =(Game &&game) noexcept
{
	destroyGraphics();
	moveData(game);

	return *this;
}

void Game::run() noexcept
{
	running = true;
	std::thread musicThread{&Game::loopMusic, this};
	
	using namespace std::chrono;
	high_resolution_clock::time_point lastTime{high_resolution_clock::now()};
#if DEBUG
	high_resolution_clock::time_point lastFpsTime{high_resolution_clock::now()};
	int fps = 0;
#endif
	while (running) [[likely]]
	{
		const high_resolution_clock::time_point now{high_resolution_clock::now()};
		delta = (now - lastTime).count() / 1e9f;
		lastTime = now;

		update();
		render();
#if DEBUG
		++fps;

		if (now - lastFpsTime >= seconds{1}) [[likely]]
		{
			LOG_INFO(SDL_LOG_CATEGORY_APPLICATION, "%i FPS\n", fps)

			lastFpsTime = now;
			fps = 0;
		}
#endif
	}

	musicThread.join();
}

void Game::setFullscreen(bool fullscreen) noexcept
{
	if (fullscreen)
	{
		SDL_DisplayMode displayMode;

		SDL_GetCurrentDisplayMode(0, &displayMode);
		SDL_SetWindowSize(window, displayMode.w, displayMode.h);

		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowSize(window, 640, 480);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
	}
}

SDL_Point Game::getWindowSize() const noexcept
{
	SDL_Point size;
	SDL_GetWindowSize(window, &size.x, &size.y);

	return size;
}

void Game::toMainMenu() noexcept
{
	gsm.clear();
	gsm.add(std::make_unique<MainMenuState>(*this));
}

void Game::update() noexcept
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		if (event.type == SDL_EventType::SDL_QUIT)
			running = false;

	gsm.update();
}

void Game::render() const noexcept
{
	gsm.render();
	SDL_RenderPresent(rdr);
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderClear(rdr);
}

void Game::loopMusic() noexcept
{
	int nextTrack = 0;

	while (running)
		if (!Mix_PlayingMusic())
		{
			Mix_PlayMusic(audioc.getMusic(nextTrack++), 0);
			Mix_VolumeMusic(13);

			if (nextTrack > 3)
				nextTrack = 0;
		}
}

void Game::moveData(Game &game) noexcept
{
	running = game.running;
	game.running = false;

	texc = std::move(game.texc);
	fontc = std::move(game.fontc);
	audioc = std::move(game.audioc);

	gsm = std::move(game.gsm);

	window = std::move(game.window);
	game.window = nullptr;

	rdr = std::move(game.rdr);
	game.rdr = nullptr;

	delta = game.delta;
	game.delta = 0.0f;
}

void Game::destroyGraphics() noexcept
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(rdr);
}
