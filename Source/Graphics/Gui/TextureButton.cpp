#include "TextureButton.h"

TextureButton::TextureButton(SDL_Rect tf, const Game &game, const Visuals &visuals) noexcept
	: Gui{tf, game},
	visuals{visuals}
{
}

void TextureButton::render(SDL_Renderer *rdr) const noexcept
{
	if (isPressed())
		SDL_RenderCopy(rdr, visuals.ss, &visuals.pressedSrcRect, &tf);
	else if (isHoveredOver())
		SDL_RenderCopy(rdr, visuals.ss, &visuals.hoveringSrcRect, &tf);
	else
		SDL_RenderCopy(rdr, visuals.ss, &visuals.normalSrcRect, &tf);
}

TextureButton::Visuals::Visuals(SDL_Texture *ss, SDL_Rect normalSrcRect, SDL_Rect pressedSrcRect, SDL_Rect hoveringSrcRect) noexcept
	: ss{ss},
	normalSrcRect{normalSrcRect},
	pressedSrcRect{pressedSrcRect},
	hoveringSrcRect{hoveringSrcRect}
{
}
