#pragma once

#include "Gui.h"
#include "../Text.h"
#include "ButtonHoverEffect.h"

#include <SDL_render.h>
#include <SDL_pixels.h>

class Game;

class TextButton final : public Gui
{
public:
	struct Properties
	{
		Text text;
		ButtonHoverEffect hoverEffect;

		Properties(Text &&text, ButtonHoverEffect hoverEffect) noexcept;
	};

	TextButton(SDL_Point pos, const Game &game, Properties &&props) noexcept;

	void update() noexcept override;
	void render(SDL_Renderer *rdr) const noexcept override;

private:
	Properties props;

	SDL_FRect normalTextTf{static_cast<float>(tf.x), static_cast<float>(tf.y), props.text.dstRect.w, props.text.dstRect.h};
	SDL_Colour normalColour = props.text.colourMod;
};