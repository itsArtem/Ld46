#pragma once

#include <SDL_render.h>

#include <string>
#include <vector>
#include <cstddef>

class TextureCache final
{
public:
	TextureCache() noexcept;

	TextureCache(const TextureCache &texc) = delete;
	TextureCache(TextureCache &&texc) noexcept;

	~TextureCache() noexcept;

	TextureCache &operator =(const TextureCache &texc) = delete;
	TextureCache &operator =(TextureCache &&texc) noexcept;

	void load(const std::string &path, SDL_Renderer *rdr);
	[[nodiscard]] SDL_Texture *get(std::size_t index) const noexcept;

	void remove(std::size_t index) noexcept;
	void clear() noexcept;

private:
	std::vector<SDL_Texture *> textures;
};