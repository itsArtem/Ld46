#include "TextButton.h"

#include <utility>
#include <iostream>

TextButton::Properties::Properties(Text &&text, ButtonHoverEffect hoverEffect) noexcept
	: text{std::move(text)},
	hoverEffect{hoverEffect}
{
}

TextButton::TextButton(SDL_Point pos, const Game &game, Properties &&props) noexcept
	: Gui{{pos.x, pos.y, 0, 0}, game},
	props{std::move(props)}
{
	tf.w = static_cast<int>(props.text.dstRect.w);
	tf.h = static_cast<int>(props.text.dstRect.h);
}

void TextButton::update() noexcept
{
	Gui::update();

	if (props.hoverEffect != ButtonHoverEffect::none)
		switch (props.hoverEffect)
		{
		case ButtonHoverEffect::enlarge:
			if (isHoveredOver())
			{
				const float offset = 4;

				props.text.dstRect.x = normalTextTf.x - offset / 2;
				props.text.dstRect.y = normalTextTf.y - offset / 4;

				props.text.dstRect.w = normalTextTf.w + offset * 2;
				props.text.dstRect.h = normalTextTf.h + offset / 2;
			}
			else
				props.text.dstRect = normalTextTf;
			break;

		case ButtonHoverEffect::dim:
			if (isHoveredOver())
			{
				props.text.colourMod.r = static_cast<int>(normalColour.r / 1.5f);
				props.text.colourMod.g = static_cast<int>(normalColour.g / 1.5f);
				props.text.colourMod.b = static_cast<int>(normalColour.b / 1.5f);
			}
			else
				props.text.colourMod = normalColour;
			break;
		}

	if (isPressed())
	{
		props.text.colourMod.r = normalColour.r / 3;
		props.text.colourMod.g = normalColour.g / 3;
		props.text.colourMod.b = normalColour.b / 3;
	}
	else if (props.hoverEffect != ButtonHoverEffect::dim)
		props.text.colourMod = normalColour;
}

void TextButton::render(SDL_Renderer *rdr) const noexcept
{
	SDL_RenderDrawRect(rdr, &tf);
	props.text.render(rdr, Text::Shadow::right);
}