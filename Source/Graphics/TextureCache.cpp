#include "TextureCache.h"

#include <SDL_image.h>
#include <SDL_error.h>

#include <utility>
#include <stdexcept>

TextureCache::TextureCache() noexcept = default;

TextureCache::TextureCache(TextureCache &&texc) noexcept
{
	textures = std::move(texc.textures);
}

TextureCache::~TextureCache() noexcept
{
	clear();
}

TextureCache &TextureCache::operator =(TextureCache &&texc) noexcept
{
	clear();

	textures = std::move(texc.textures);
	texc.textures.clear();

	return *this;
}

void TextureCache::load(const std::string &path, SDL_Renderer *rdr)
{
	SDL_Texture *const texture = IMG_LoadTexture(rdr, path.c_str());

	if (!texture) [[unlikely]]
		throw std::runtime_error{SDL_GetError()};

	textures.emplace_back(texture);
}

SDL_Texture *TextureCache::get(std::size_t index) const noexcept
{
	return textures[index];
}

void TextureCache::remove(std::size_t index) noexcept
{
	SDL_DestroyTexture(textures[index]);
	textures.erase(textures.begin() + index);
}

void TextureCache::clear() noexcept
{
	for (SDL_Texture *texture : textures) 
		SDL_DestroyTexture(texture);

	textures.clear();
}