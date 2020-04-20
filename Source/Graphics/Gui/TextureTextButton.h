#pragma once

#include "Gui.h"
#include "../Text.h"
#include "ButtonHoverEffect.h"

#include <SDL_render.h>
#include <SDL_pixels.h>

class Game;

class TextureTextButton final : public Gui
{
public:
	struct Properties
	{
		SDL_Texture *const texture;

		const SDL_Rect normalSrcRect;
		const SDL_Rect pressedSrcRect;
		const SDL_Rect hoveringSrcRect;

		Text text;
		const SDL_FRect normalTextDstRect{text.dstRect};
		const SDL_Colour normaltc{text.colourMod};

		ButtonHoverEffect hoverEffect;

		Properties(SDL_Texture *texture, SDL_Rect srcRect, SDL_Rect pressedSrcRect, SDL_Rect hoveringSrcRect, SDL_FRect dstRect, Text &&text, ButtonHoverEffect buttonHoverEffect) noexcept;
	};

	TextureTextButton(SDL_Rect tf, const Game &game, Properties &&props) noexcept;
	
	void update() noexcept;
	void render(SDL_Renderer *rdr) const noexcept;

private:
	Properties props;
	SDL_Colour colourMod{255, 255, 255, 255};

	const SDL_Rect normalTf{tf};

	void setPosition(SDL_FPoint pos) noexcept;
};