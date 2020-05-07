#pragma once

#include "Gui.h"

#include <SDL_render.h>

#include <vector>
#include <memory>
#include <cstddef>

class GuiCanvas final
{
public:
	void add(std::unique_ptr<Gui> gui);
	void remove(std::size_t index) noexcept;

	[[nodiscard]] Gui &get(std::size_t index) noexcept;

	void update() noexcept;
	void render(SDL_Renderer *rdr) const noexcept;

	std::size_t getSize() const noexcept;

private:
	std::vector<std::unique_ptr<Gui>> elements;
};