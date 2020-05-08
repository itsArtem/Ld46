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
#include <cstddef>

Game::Game()
{
	if (!window) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	SDL_SetWindowMinimumSize(window, 1100, 680);

	SDL_Surface *icon = IMG_Load("Resources/Textures/Icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	if (config.doesFileExist())
		config.load();
	else 
		config.save();

	if (config.fullscreen)
		setFullscreen(true);

	int rdrFlags;

	if (config.vSync)
		rdrFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	else
		rdrFlags = SDL_RENDERER_ACCELERATED;

	rdr = SDL_CreateRenderer(window, -1, rdrFlags);
	if (!rdr) [[unlikely]]
	{
		SDL_DestroyWindow(window);
		throw std::runtime_error{SDL_GetError()};
	}
	SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);

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
	audioc.loadMusic("Resources/Audio/Music/Track5.mid");
	audioc.loadMusic("Resources/Audio/Music/Track6.mid");
	audioc.loadMusic("Resources/Audio/Music/Track7.mid");
	audioc.loadMusic("Resources/Audio/Music/Track8.mid");

	audioc.loadChunk("Resources/Audio/Chunks/Attack.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Break.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hit.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hostile.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Hurt.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Place.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Remove.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Select.wav");
	audioc.loadChunk("Resources/Audio/Chunks/CantPlace.wav");
	audioc.loadChunk("Resources/Audio/Chunks/Tip.wav");

	Mix_VolumeMusic(1);

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
	std::size_t nextTrack = 0;
	Mix_PlayMusic(audioc.getMusic(nextTrack++), 0);
	using namespace std::chrono;
	steady_clock::time_point start{steady_clock::now()};
	steady_clock::duration duration = minutes{1} + seconds{45};

	while (running)
	{
		/* 
		Mix_Playing() tries to read or write to a bad memory address, so I have to manually check if something is playing. 
		Not sure if it's just me or an SDL Mixer bug.
		*/	
		const steady_clock::time_point now{steady_clock::now()};
		if (now - start >= duration)
		{
			switch (nextTrack)
			{
			case 0:
				duration = minutes{1} + seconds{45};
				break;
			case 1:
				duration = minutes{1} + seconds{19};
				break;
			case 2:
				duration = minutes{1} + seconds{39};
				break;
			case 3:
				duration = minutes{1} + seconds{43};
				break;
			case 4:
			case 6:
				duration = minutes{3} + seconds{1};
				break;
			case 5:
				duration = minutes{3} + seconds{15};
				break;
			case 7:
				duration = minutes{2} + seconds{34};
				break;
			}

			Mix_PlayMusic(audioc.getMusic(nextTrack++), 0);
			start = now;

			if (nextTrack > audioc.getMusicCount())
				nextTrack = 0;
		}
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
