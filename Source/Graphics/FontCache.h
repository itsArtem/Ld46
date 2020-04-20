#pragma once

#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <cstddef>

class FontCache final
{
public:
	FontCache() noexcept;
	
	FontCache(const FontCache &fontc) = delete;
	FontCache(FontCache &&fontc) noexcept;

	~FontCache();

	FontCache &operator =(const FontCache &fontc) = delete;
	FontCache &operator =(FontCache &&fontc) noexcept;
	
	void open(const std::string &path, int size);
	
	void close(std::size_t index) noexcept;
	void clear() noexcept;

	[[nodiscard]] TTF_Font *get(std::size_t index) const noexcept;

private:
	std::vector<TTF_Font *> fonts;

	void moveData(FontCache &fontc) noexcept;
};