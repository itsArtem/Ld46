#pragma once

#include <SDL_render.h>

#include <vector>

class Entity;
class EntityManager;

class RenderSystem final
{
public:
	explicit RenderSystem(EntityManager &em) noexcept;

	void update(Entity &e, float delta) const noexcept;
	void render(SDL_Renderer *rdr) const noexcept;

private:
	EntityManager &em;
};