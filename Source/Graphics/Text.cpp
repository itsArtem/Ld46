#include "Text.h"

#include <SDL_error.h>

#include <stdexcept>
#include <cctype>

Text::Text() = default;

Text::Text(TTF_Font *font, const std::string &string, SDL_Colour colour, bool blended, SDL_Renderer *rdr)
{
	setText(font, string, colour, blended, rdr);
}

Text::Text(TTF_Font *font, const std::string &string, SDL_Colour fg, SDL_Colour bg, SDL_Renderer *rdr)
{
	setShadedText(font, string, fg, bg, rdr);
}

Text::Text(TTF_Font *font, const std::string &string, SDL_Colour colour, std::uint32_t wrap, SDL_Renderer *rdr)
{
	setWrappedText(font, string, colour, wrap, rdr);
}

Text::Text(Text &&text) noexcept
{
	moveData(text);
}

Text::~Text()
{
	SDL_DestroyTexture(texture);
}

Text &Text::operator=(Text &&text) noexcept
{
	SDL_DestroyTexture(texture);
	moveData(text);

	return *this;
}

void Text::setText(TTF_Font *font, const std::string &string, SDL_Colour colour, bool blended, SDL_Renderer *rdr)
{
	SDL_Surface *surface;

	if (blended)
		surface = TTF_RenderText_Blended(font, string.c_str(), colour);
	else
		surface = TTF_RenderText_Solid(font, string.c_str(), colour);

	if (!surface) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	createText(surface, rdr);
	SDL_FreeSurface(surface);

	setSize(font, string);
}

void Text::setShadedText(TTF_Font *font, const std::string &string, SDL_Colour fg, SDL_Colour bg, SDL_Renderer *rdr)
{
	SDL_Surface *const surface = TTF_RenderText_Shaded(font, string.c_str(), fg, bg);

	if (!surface) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	createText(surface, rdr);
	SDL_FreeSurface(surface);

	setSize(font, string);
}

void Text::setWrappedText(TTF_Font *font, const std::string &string, SDL_Colour colour, std::uint32_t wrap, SDL_Renderer *rdr)
{
	SDL_Surface *const surface = TTF_RenderText_Blended_Wrapped(font, string.c_str(), colour, wrap);

	if (!surface) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	createText(surface, rdr);
	SDL_FreeSurface(surface);
}

void Text::render(SDL_Renderer *rdr, Shadow shadow, double angle, SDL_FPoint *centre, SDL_RendererFlip flip) const noexcept
{
	if (shadow != Shadow::none)
	{
		SDL_FRect sdstRect{dstRect.x + (shadow == Shadow::left ? -2 : 2), dstRect.y + 2, dstRect.w, dstRect.h};

		SDL_SetTextureColorMod(texture, colourMod.r / 8, colourMod.g / 8, colourMod.b / 8);
		SDL_RenderCopyExF(rdr, texture, nullptr, &sdstRect, angle, centre, flip);
	}

	SDL_SetTextureColorMod(texture, colourMod.r, colourMod.g, colourMod.b);
	SDL_SetTextureAlphaMod(texture, colourMod.a);

	SDL_RenderCopyExF(rdr, texture, nullptr, &dstRect, angle, centre, flip);
}

void Text::createText(SDL_Surface *surface, SDL_Renderer *rdr)
{
	if (texture) SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(rdr, surface);

	if (!texture) [[unlikely]] 
		throw std::runtime_error{SDL_GetError()};
}

void Text::setSize(TTF_Font *font, const std::string &text) noexcept
{
	SDL_Point size;
	TTF_SizeText(font, text.c_str(), &size.x, &size.y);

	dstRect.w = static_cast<float>(size.x);
	dstRect.h = static_cast<float>(size.y);
}

void Text::moveData(Text &text) noexcept
{
	dstRect = std::move(text.dstRect);
	colourMod = std::move(text.colourMod);

	texture = std::move(text.texture);
	text.texture = nullptr;
}