#include "RenderSystem.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Component.h"
#include "../World.h"
#include "../Tile/Tile.h"
#include "../../Game.h"

#include <algorithm>

RenderSystem::RenderSystem(EntityManager &em) noexcept
	: em{em}
{
}

void RenderSystem::update(Entity &e, float delta) const noexcept
{
	DirectionalAnimationComponent *const dac = e.getComponent<DirectionalAnimationComponent>();
	SingleAxisAnimationComponent *const saac = e.getComponent<SingleAxisAnimationComponent>();
	VelocityComponent *const vc = e.getComponent<VelocityComponent>();
	ProjectileComponent *const pjc = e.getComponent<ProjectileComponent>();

	if (pjc)
	{
		pjc->timer -= 1 * delta;
		if (pjc->timer <= 0.0f)
			pjc->alpha -= 80 * delta;
	}

	if (!dac && !saac || !vc) return;

	if (dac)
	{
		if (dac && vc->velocity.x < 0.0f)
		{
			dac->dir = DirectionalAnimationComponent::Direction::left;
		}
		else if (dac && vc->velocity.x > 0.0f)
		{
			dac->dir = DirectionalAnimationComponent::Direction::right;
		}
		else if (vc->velocity.y < 0.0f)
		{
			dac->dir = DirectionalAnimationComponent::Direction::up;
		}
		else if (vc->velocity.y > 0.0f)
		{
			dac->dir = DirectionalAnimationComponent::Direction::down;
		}
	}
	else
	{
		if (vc->velocity.x < 0.0f || vc->velocity.y < 0.0f)
		{
			saac->dir = SingleAxisAnimationComponent::Direction::neg;
		}
		else if (vc->velocity.x > 0.0f || vc->velocity.y > 0.0f)
		{
			saac->dir = SingleAxisAnimationComponent::Direction::pos;
		}
	}

	if (vc->velocity.x != 0.0f || vc->velocity.y != 0.0f)
	{
		if (dac)
		{
			switch (dac->dir)
			{
			case DirectionalAnimationComponent::Direction::up:
				dac->up.update(delta);
				break;

			case DirectionalAnimationComponent::Direction::down:
				dac->down.update(delta);
				break;

			case DirectionalAnimationComponent::Direction::left:
				dac->left.update(delta);
				break;

			case DirectionalAnimationComponent::Direction::right:
				dac->right.update(delta);
				break;
			}
		}
		else
		{
			switch (saac->dir)
			{
			case SingleAxisAnimationComponent::Direction::neg:
				saac->first.update(delta);
				break;

			case SingleAxisAnimationComponent::Direction::pos:
				saac->second.update(delta);
				break;
			}
		}
	}
}

void RenderSystem::render(SDL_Renderer *rdr) const noexcept
{
	std::sort(em.renderable.begin(), em.renderable.end(), [](const Entity *first, const Entity *second)
		{
			const TransformComponent *tfc1 = first->getComponent<TransformComponent>();
			const TransformComponent *tfc2 = second->getComponent<TransformComponent>();

			return tfc1->tf.y < tfc2->tf.y;
		});

	for (Entity *e : em.renderable)
	{
		const TransformComponent *const tfc = e->getComponent<TransformComponent>();
		const TextureComponent *const tc = e->getComponent<TextureComponent>();
		const AngledTextureComponent *const atc = e->getComponent<AngledTextureComponent>();
		const DirectionalAnimationComponent *const dac = e->getComponent<DirectionalAnimationComponent>();
		const SingleAxisAnimationComponent *const saac = e->getComponent<SingleAxisAnimationComponent>();
		const VelocityComponent *const vc = e->getComponent<VelocityComponent>();

		if (tc)
		{
			const SDL_FRect dstRect{tfc->tf.x - tc->world.camera.x, tfc->tf.y - tc->world.camera.y, tfc->tf.w, tfc->tf.h};
			SDL_RenderCopyF(rdr, tc->texture, &tc->srcRect, &dstRect);
		}

		if (atc)
		{
			const SDL_FRect dstRect{tfc->tf.x - atc->world.camera.x, tfc->tf.y - atc->world.camera.y, tfc->tf.w, tfc->tf.h};
			SDL_RenderCopyExF(rdr, atc->texture, &atc->srcRect, &dstRect, atc->angle, nullptr, SDL_FLIP_NONE);
		}

		if (vc)
		{
			if (dac)
			{
				SDL_SetTextureColorMod(dac->ss, 255, 255, 255);
				const SDL_FRect dstRect{tfc->tf.x - dac->world.camera.x, tfc->tf.y - dac->world.camera.y, tfc->tf.w, tfc->tf.h};

				switch (dac->dir)
				{
				case DirectionalAnimationComponent::Direction::up:
					if (vc->velocity.y < 0.0f)
						dac->up.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, dac->ss, &dac->idleUpSrcRect, &dstRect);

					break;

				case DirectionalAnimationComponent::Direction::down:
					if (vc->velocity.y > 0.0f)
						dac->down.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, dac->ss, &dac->idleDownSrcRect, &dstRect);

					break;

				case DirectionalAnimationComponent::Direction::left:
					if (vc->velocity.x < 0.0f)
						dac->left.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, dac->ss, &dac->idleLeftSrcRect, &dstRect);

					break;

				case DirectionalAnimationComponent::Direction::right:
					if (vc->velocity.x > 0.0f)
						dac->right.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, dac->ss, &dac->idleRightSrcRect, &dstRect);

					break;
				}
			}
			else if (saac)
			{
				SDL_SetTextureColorMod(saac->ss, 255, 255, 255);
				const SDL_FRect dstRect{tfc->tf.x - saac->world.camera.x, tfc->tf.y - saac->world.camera.y, tfc->tf.w, tfc->tf.h};

				switch (saac->dir)
				{
				case SingleAxisAnimationComponent::Direction::neg:
					if (vc->velocity.x < 0.0f || vc->velocity.y < 0.0f)
						saac->first.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, saac->ss, &saac->idleFirstSrcRect, &dstRect);
					break;

				case SingleAxisAnimationComponent::Direction::pos:
					if (vc->velocity.x > 0.0f || vc->velocity.y > 0.0f)
						saac->second.render(rdr, dstRect);
					else
						SDL_RenderCopyF(rdr, saac->ss, &saac->idleSecondSrcRect, &dstRect);
					break;
				}
			}
		}
	}
}