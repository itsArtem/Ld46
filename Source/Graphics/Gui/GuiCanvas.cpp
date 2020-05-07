#include "GuiCanvas.h"

#include <utility>

void GuiCanvas::add(std::unique_ptr<Gui> gui)
{
	elements.emplace_back(std::move(gui));
}

void GuiCanvas::remove(std::size_t index) noexcept
{
	elements.erase(elements.begin() + index);
}

Gui &GuiCanvas::get(std::size_t index) noexcept
{
	return *elements[index];
}

void GuiCanvas::update() noexcept
{
	for (auto &e : elements)
		e->update();
}

void GuiCanvas::render(SDL_Renderer *rdr) const noexcept
{
	for (const auto &e : elements)
		e->render(rdr);
}
