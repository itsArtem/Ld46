#pragma once

#include <SDL_render.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>

#include <chrono>
#include <cstdint>

class Game;

class Gui
{
public:
	SDL_Rect tf;

	Gui(const Gui &gui) noexcept = default;
	Gui(Gui &&gui) noexcept = default;
	
	virtual ~Gui() = default;

	Gui &operator =(const Gui &gui) noexcept = default;
	Gui &operator =(Gui && gui) noexcept = default;

	virtual void update() noexcept;
	virtual void render(SDL_Renderer *rdr) const noexcept = 0;

	[[nodiscard]] bool isHoveredOver() const noexcept { return hoveredOver; };

	[[nodiscard]] bool isPressed() const noexcept { return pressed; };
	[[nodiscard]] bool isReleased() const noexcept { return released; };

protected:
	explicit Gui(SDL_Rect tf, const Game &game) noexcept;

private:
	const Game &game;

	SDL_Point mouse;
	std::uint32_t button = SDL_GetMouseState(&mouse.x, &mouse.y);

	bool hoveredOver = false;

	bool pressed = false;
	bool released = false;

	SDL_Point click{-1, -1};
	std::chrono::steady_clock::duration clickTime{std::chrono::milliseconds::zero()};
};