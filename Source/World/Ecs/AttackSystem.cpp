#include "AttackSystem.h"
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"
#include "../World.h"
#include "../../Game.h"

#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_mixer.h>

#include <cstdint>
#include <cmath>
#include <iostream>

void AttackSystem::update(Entity &e, float delta) noexcept
{
	PlayerComponent *const pc = e.getComponent<PlayerComponent>();
	const TransformComponent *const tfc = e.getComponent<TransformComponent>();

	if (pc && tfc && !pc->preparation)
	{
		SDL_Point mouse;
		const std::uint32_t button = SDL_GetMouseState(&mouse.x, &mouse.y);

		pc->attackTimer -= 1000 * delta;

		if (button == SDL_BUTTON_LMASK && pc->attackTimer <= 0.0f)
		{
			const float speed = 350.0f;
			const SDL_FPoint start{tfc->tf.x + tfc->tf.w / 2 - 18, tfc->tf.y + tfc->tf.h / 2 - 18};

			const double angle = std::atan2(mouse.y + static_cast<double>(pc->world.camera.y) - start.y,
				mouse.x + static_cast<double>(pc->world.camera.x) - start.x) * -180 / M_PI;

			const SDL_FPoint velocity{static_cast<float>(std::cos(angle * M_PI / -180) * speed),
				static_cast<float>(std::sin(angle * M_PI / -180) * speed)};

			createProjectile(pc->em, Projectile::arrow, start, velocity, speed, true, pc->world, pc->game);
			pc->attackTimer = pc->attackDelay;

			Mix_PlayChannel(0, pc->game.audioc.getChunk(0), 0);
			Mix_Volume(0, 50);
		}
	}
	else
	{
		ProjectileComponent *const pjc = e.getComponent<ProjectileComponent>();
		const CollisionComponent *const cc = e.getComponent<CollisionComponent>();

		if (pjc && cc && !pjc->hit)
			if (cc->entityCollided != -1)
			{
				Entity *oe = pjc->em.getEntity(cc->entityCollided);
				if (oe)
				{
					HealthComponent *const ohc = oe->getComponent<HealthComponent>();

					if (ohc)
					{
						ohc->health -= pjc->damage;
						pjc->hit = true;

						if (pjc->friendly)
						{
							Entity *player = cc->world.em.getEntity(cc->world.getPlayer());

							if (player)
							{
								PlayerComponent *const pc = player->getComponent<PlayerComponent>();
								if (pc)
								{
									pc->points += 10;
									pc->score += 10;

									Mix_PlayChannel(2, pc->game.audioc.getChunk(2), 0);
								}
							}
							else
								Mix_PlayChannel(4, pjc->game.audioc.getChunk(4), 0);
						}
					}
				}
			}
	}
}
