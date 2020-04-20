#pragma once

#include "Gui.h"

class Game;

class TextureButton final : public Gui
{
public:
	struct Visuals
	{
		SDL_Texture *const ss; 
		
		const SDL_Rect normalSrcRect;
		const SDL_Rect pressedSrcRect;
		const SDL_Rect hoveringSrcRect;

		Visuals(SDL_Texture *ss, SDL_Rect normalSrcRect, SDL_Rect pressedSrcRect, SDL_Rect hoveringSrcRect) noexcept;
	};

	TextureButton(SDL_Rect tf, const Game &game, const Visuals &visuals) noexcept;
	void render(SDL_Renderer *rdr) const noexcept override;

private:
	Visuals visuals;
};