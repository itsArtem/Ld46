#include "TextureTextButton.h"

#include <utility>

TextureTextButton::Properties::Properties(SDL_Texture *texture, SDL_Rect srcRect, SDL_Rect clickSrcRect, SDL_Rect hoveringSrcRect, SDL_FRect dstRect, Text &&text, ButtonHoverEffect hoverEffect) noexcept
	: texture{texture},
	normalSrcRect{srcRect},
	pressedSrcRect{clickSrcRect},
	hoveringSrcRect{hoveringSrcRect},
	text{std::move(text)},
	hoverEffect{hoverEffect}
{
	this->text.dstRect.x = dstRect.x + dstRect.w / 2 - text.dstRect.w / 2;
	this->text.dstRect.y = dstRect.y + dstRect.h / 2 - text.dstRect.h / 2;
}

TextureTextButton::TextureTextButton(SDL_Rect tf, const Game &game, Properties &&props) noexcept
	: Gui{tf, game},
	props{std::move(props)}
{
}

void TextureTextButton::update() noexcept
{
	Gui::update();

	if (props.hoverEffect != ButtonHoverEffect::none)
		switch (props.hoverEffect)
		{
		case ButtonHoverEffect::enlarge:
			if (isHoveredOver())
			{
				const float offset = 8;

				tf.w = static_cast<int>(normalTf.w + offset);
				tf.h = static_cast<int>(normalTf.h + offset / 2);

				props.text.dstRect.w = props.normalTextDstRect.w + offset;
				props.text.dstRect.h = props.normalTextDstRect.h + offset / 2;

				setPosition({normalTf.x - offset / 2, normalTf.y - offset / 4});
			}
			else
			{
				tf.w = normalTf.w;
				tf.h = normalTf.h;

				props.text.dstRect.w = props.normalTextDstRect.w;
				props.text.dstRect.h = props.normalTextDstRect.h;

				setPosition({static_cast<float>(normalTf.x), static_cast<float>(normalTf.y)});
			}

			tf.w = static_cast<int>(normalTf.w);
			tf.h = static_cast<int>(normalTf.h);

			break;

		case ButtonHoverEffect::dim:
			if (isHoveredOver())
			{
				colourMod.r = colourMod.g = colourMod.b = 170;

				props.text.colourMod.r = static_cast<int>(props.normaltc.r / 1.5f);
				props.text.colourMod.g = static_cast<int>(props.normaltc.g / 1.5f);
				props.text.colourMod.b = static_cast<int>(props.normaltc.b / 1.5f);
			}
			else
			{
				colourMod.r = colourMod.g = colourMod.b = 255;
				props.text.colourMod = props.normaltc;
			}

			break;
		}

	if (isPressed())
	{
		props.text.colourMod.r = props.normaltc.r / 3;
		props.text.colourMod.g = props.normaltc.g / 3;
		props.text.colourMod.b = props.normaltc.b / 3;
	}
	else if (props.hoverEffect != ButtonHoverEffect::dim)
		props.text.colourMod = props.normaltc;
}

void TextureTextButton::render(SDL_Renderer *rdr) const noexcept
{
	SDL_SetTextureColorMod(props.texture, colourMod.r, colourMod.g, colourMod.b);

	SDL_RenderCopy(rdr, props.texture, isPressed() ? &props.pressedSrcRect : &props.normalSrcRect, &normalTf);
	props.text.render(rdr, Text::Shadow::right);
}

void TextureTextButton::setPosition(SDL_FPoint pos) noexcept
{
	tf.x = static_cast<int>(pos.x);
	tf.y = static_cast<int>(pos.y);

	props.text.dstRect.x = tf.x + tf.w / 2 - props.text.dstRect.w / 2;
	props.text.dstRect.y = tf.y + tf.h / 2 - props.text.dstRect.h / 2;

	tf.x = static_cast<int>(pos.x);
	tf.y = static_cast<int>(pos.y);
}
