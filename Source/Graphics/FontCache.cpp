#include "FontCache.h"

#include <SDL_error.h>

#include <utility>
#include <stdexcept>

FontCache::FontCache() noexcept = default;

FontCache::FontCache(FontCache &&fontc) noexcept
{
	moveData(fontc);
}

FontCache::~FontCache()
{
	clear();
}

FontCache &FontCache::operator=(FontCache &&fontc) noexcept
{
	clear();
	moveData(fontc);
	
	return *this;
}

void FontCache::open(const std::string &path, int size)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);

	if (!font) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	fonts.emplace_back(font);
}

void FontCache::close(std::size_t index) noexcept
{
	TTF_CloseFont(fonts[index]);
	fonts.erase(fonts.begin() + index);
}

void FontCache::clear() noexcept
{
	for (TTF_Font *font : fonts)
		TTF_CloseFont(font);

	fonts.clear();
}

TTF_Font *FontCache::get(std::size_t index) const noexcept
{
	return fonts[index];
}

void FontCache::moveData(FontCache &fontc) noexcept
{
	fonts = std::move(fontc.fonts);
	fontc.fonts.clear();
}
