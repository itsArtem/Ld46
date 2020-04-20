#pragma once

#include <SDL_ttf.h>
#include <SDL_rect.h>

#include <string>
#include <cstdint>

class Text final
{
public:
	SDL_FRect dstRect{0, 0, 0, 0};
	SDL_Colour colourMod{255, 255, 255, 255};

	enum class Shadow
	{
		none, left, right
	};
	
	Text();
	Text(TTF_Font *font, const std::string &string, SDL_Colour colour, bool blended, SDL_Renderer *rdr);
	Text(TTF_Font *font, const std::string &string, SDL_Colour fg, SDL_Colour bg, SDL_Renderer *rdr);
	Text(TTF_Font *font, const std::string &string, SDL_Colour colour, std::uint32_t wrap, SDL_Renderer *rdr);
	
	Text(const Text &text) = delete;
	Text(Text &&text) noexcept;

	~Text();

	Text &operator =(const Text &text) = delete;
	Text &operator =(Text &&text) noexcept;

	void setText(TTF_Font *font, const std::string &string, SDL_Colour colour, bool blended, SDL_Renderer *rdr);
	void setShadedText(TTF_Font *font, const std::string &string, SDL_Colour fg, SDL_Colour bg, SDL_Renderer *rdr);
	void setWrappedText(TTF_Font *font, const std::string &string, SDL_Colour colour, std::uint32_t wrap, SDL_Renderer *rdr);

	void render(SDL_Renderer *rdr, Shadow shadow, double angle = 0.0, SDL_FPoint *centre = nullptr, 
		SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) const noexcept;

private:
	SDL_Texture *texture = nullptr;
	
	void createText(SDL_Surface *surface, SDL_Renderer *rdr);
	void setSize(TTF_Font *font, const std::string &string) noexcept;

	void moveData(Text &text) noexcept;
};