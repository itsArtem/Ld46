#include "Animation.h"

Animation::Animation(SDL_Texture *ss, SDL_Rect start, int frames, float msPerFrame) noexcept
	: ss{ss},
	srcRect{start},
	frames{frames},
	msPerFrame{msPerFrame}
{
}

void Animation::update(float delta) noexcept
{
	time += 1000 * delta;

	if (time < msPerFrame) return;
	time = 0.0f;

	srcRect.x += srcRect.w;
	
	if (srcRect.x > end)
		srcRect.x = start;
}

void Animation::render(SDL_Renderer *rdr, SDL_FRect dstRect, double angle, const SDL_FPoint *centre, SDL_RendererFlip flip) const noexcept
{
	SDL_SetTextureColorMod(ss, 255, 255, 255);
	SDL_RenderCopyExF(rdr, ss, &srcRect, &dstRect, angle, centre, flip);
}