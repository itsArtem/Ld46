#include "HostileSystem.h"
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"
#include "../World.h"

#include <SDL_mixer.h>

HostileSystem::HostileSystem(EntityManager &em) noexcept
	: em{em}
{
}

void HostileSystem::update(Entity &e, float delta) noexcept
{
	MeleeHostileComponent *const mhc = e.getComponent<MeleeHostileComponent>();
	RangedHostileComponent *const rhc = e.getComponent<RangedHostileComponent>();
	const CollisionComponent *const cc = e.getComponent<CollisionComponent>();

	if (cc)
	{
		if (mhc)
		{
			Entity *target;
			if (cc->entityCollided == cc->world.getPlayer())
				target = cc->world.em.getEntity(cc->world.getPlayer());
			else if (cc->entityCollided == cc->world.getChest())
				target = cc->world.em.getEntity(cc->world.getChest());
			else
				target = em.getEntity(mhc->target);;

			if (!target) return;

			HealthComponent *const hc = target->getComponent<HealthComponent>();

			if (hc && (cc->entityCollided == target->id))
			{
				mhc->attackTimer -= 1 * delta;

				if (mhc->attackTimer <= 0.0f)
				{
					hc->health -= mhc->damage;
					mhc->attackTimer = mhc->attackDelay;
					Mix_PlayChannel(4, mhc->game.audioc.getChunk(4), 0);
				}
			}
		}
		else if (rhc)
		{
			Entity *target = em.getEntity(rhc->target);
			if (!target) return;
			rhc->attackTimer -= 1 * delta;

			if (rhc->attackTimer <= 0.0f)
			{
				TransformComponent *tfc = e.getComponent<TransformComponent>();
				TransformComponent *ptfc = target->getComponent<TransformComponent>();
				rhc->attackTimer = rhc->attackDelay;

				const float speed = 450.0f;
				const SDL_FPoint start{tfc->tf.x + tfc->tf.w / 2 - 18, tfc->tf.y + tfc->tf.h / 2 - 18};
				const SDL_FPoint dst{ptfc->tf.x - tfc->tf.x, ptfc->tf.y - tfc->tf.y};

				const double angle = std::atan2(dst.y, dst.x) * -180 / M_PI;

				const SDL_FPoint velocity{static_cast<float>(std::cos(angle * M_PI / -180) * speed),
					static_cast<float>(std::sin(angle * M_PI / -180) * speed)};

				createProjectile(rhc->em, rhc->projectile, start, velocity, speed, false, cc->world, rhc->game);
				rhc->attackTimer = rhc->attackDelay;

				Mix_PlayChannel(3, rhc->game.audioc.getChunk(3), 0);
			}
		}
	}
}
