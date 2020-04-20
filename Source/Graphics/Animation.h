#pragma once

#include <SDL_render.h>
#include <SDL_rect.h>

class Animation final
{
public:
	Animation(SDL_Texture *ss, SDL_Rect start, int frames, float msPerFrame) noexcept;

	void update(float delta) noexcept;
	void render(SDL_Renderer *rdr, SDL_FRect dstRect, double angle = 0.0, const SDL_FPoint *centre = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const noexcept;

private:
	SDL_Texture *ss;

	SDL_Rect srcRect;
	const int frames;

	const int start = srcRect.x;
	const int end = srcRect.x + srcRect.w * (frames - 1);

	const float msPerFrame;
	float time = 0.0f;
};