#include "MovementSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Component.h"
#include "../World.h"
#include "../Tile/Tile.h"
#include "../../Random.h"

#include <SDL_keyboard.h>
#include <SDL_rect.h>

#include <functional>
#include <cstdint>
#include <cmath>

MovementSystem::MovementSystem(EntityManager &em)
	: em{em}
{
}

void MovementSystem::update(Entity &e, float delta) const noexcept
{
	TransformComponent *const tfc = e.getComponent<TransformComponent>();
	VelocityComponent *const vc = e.getComponent<VelocityComponent>();

	if (!tfc || !vc) return;

	PlayerComponent *const pc = e.getComponent<PlayerComponent>();

	RandomMovementComponent *const rmc = e.getComponent<RandomMovementComponent>();
	FollowComponent *const fc = e.getComponent<FollowComponent>();

	std::function<void(MovementType, MovementType)> setVelocity = [vc, delta](MovementType x, MovementType y)
	{
		if (y == MovementType::dir1)
		{
			vc->velocity.y -= vc->accel * delta;
		}
		else if (vc->velocity.y < 0.0f)
		{
			vc->velocity.y += vc->slowdown * delta;

			if (vc->velocity.y > -1.0f)
				vc->velocity.y = 0.0f;
		}
		else if (y == MovementType::dir2)
		{
			vc->velocity.y += vc->accel * delta;
		}
		else if (vc->velocity.y > 0.0f)
		{
			vc->velocity.y -= vc->slowdown * delta;

			if (vc->velocity.y < 1.0f)
				vc->velocity.y = 0.0f;
		}

		if (x == MovementType::dir1)
		{
			vc->velocity.x -= vc->accel * delta;
		}
		else if (vc->velocity.x < 0.0f)
		{
			vc->velocity.x += vc->slowdown * delta;

			if (vc->velocity.x > -1.0f)
				vc->velocity.x = 0.0f;
		}
		else if (x == MovementType::dir2)
		{
			vc->velocity.x += vc->accel * delta;
		}
		else if (vc->velocity.x > 0.0f)
		{
			vc->velocity.x -= vc->slowdown * delta;

			if (vc->velocity.x < 1.0f)
				vc->velocity.x = 0.0f;
		}
	};

	if (pc)
	{
		const std::uint8_t *const keyboard = SDL_GetKeyboardState(nullptr);

		MovementType x, y;

		if (keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S])
			y = MovementType::dir1;
		else if (keyboard[SDL_SCANCODE_S] && !keyboard[SDL_SCANCODE_W])
			y = MovementType::dir2;
		else
			y = MovementType::none;

		if (keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D])
			x = MovementType::dir1;
		else if (keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A])
			x = MovementType::dir2;
		else
			x = MovementType::none;

		setVelocity(x, y);
	}

	if (rmc)
	{
		if (getRandomInt(0, 50) == 0)
		{
			if (getRandomInt(0, 5) == 0)
				rmc->x = static_cast<MovementType>(getRandomInt(0, 2));
			else
				rmc->x = MovementType::none;

			if (getRandomInt(0, 5) == 0)
				rmc->y = static_cast<MovementType>(getRandomInt(0, 2));
			else
				rmc->y = MovementType::none;
		}

		setVelocity(rmc->x, rmc->y);
	}

	if (fc && em.getEntity(fc->target))
	{
		TransformComponent *const ttfc = em.getEntity(fc->target)->getComponent<TransformComponent>();

		if (ttfc)
		{
			const SDL_FPoint diff{ttfc->tf.x - tfc->tf.x, ttfc->tf.y - tfc->tf.y};


			MovementType mx = MovementType::none;
			MovementType my = MovementType::none;

			if (std::abs(diff.x) > fc->stopDistance)
			{
				if (diff.x < -1.0f)
					mx = MovementType::dir1;
				else if (diff.x > 1.0f)
					mx = MovementType::dir2;
			}

			if (std::abs(diff.y) > fc->stopDistance)
			{
				if (diff.y < -1.0f)
					my = MovementType::dir1;
				else if (diff.y > 1.0f)
					my = MovementType::dir2;
			}

			setVelocity(mx, my);
		}
	}

	if (vc->velocity.x == 0.0f && vc->velocity.y == 0.0f)
		return;

	if (vc->velocity.x < -vc->maxSpeed)
		vc->velocity.x = -vc->maxSpeed;
	else if (vc->velocity.x > vc->maxSpeed)
		vc->velocity.x = vc->maxSpeed;

	if (vc->velocity.y < -vc->maxSpeed)
		vc->velocity.y = -vc->maxSpeed;
	else if (vc->velocity.y > vc->maxSpeed)
		vc->velocity.y = vc->maxSpeed;

	CollisionComponent *const cc = e.getComponent<CollisionComponent>();
	ProjectileComponent *const pjc = e.getComponent<ProjectileComponent>();

	if (pjc && pjc->stop)
		return;

	if (cc)
	{
		bool xec = false, yec = false;

		for (auto *oe : em.collidables)
		{
			if (oe->id == e.id)
				continue;

			TransformComponent *const otfc = oe->getComponent<TransformComponent>();
			CollisionComponent *const occ = oe->getComponent<CollisionComponent>();
			ProjectileComponent *const opjc = oe->getComponent<ProjectileComponent>();

			if ((pjc && ((pjc->friendly && (oe->id == cc->world.getPlayer() || oe->id == cc->world.getChest())) || opjc))
				|| (pjc && ((!pjc->friendly && oe->id != cc->world.getPlayer() && oe->id != cc->world.getChest()) || opjc))
				|| !pjc && opjc)
				continue;



			const SDL_Rect cb{static_cast<int>(otfc->tf.x) + occ->bounds.x, static_cast<int>(otfc->tf.y) + occ->bounds.y + occ->bounds.h / 2, occ->bounds.w, occ->bounds.h / 2 + 8};

			if (vc->velocity.x != 0.0f)
			{
				const SDL_Rect dstRect{static_cast<int>(tfc->tf.x + cc->bounds.x + vc->velocity.x * delta),
									   static_cast<int>(tfc->tf.y + cc->bounds.y + cc->bounds.h / 2),
									   cc->bounds.w,
									   cc->bounds.h / 2 + 8};

				if (SDL_HasIntersection(&dstRect, &cb))
					xec = true;
			}

			if (vc->velocity.y != 0.0f)
			{
				const SDL_Rect dstRect{static_cast<int>(tfc->tf.x + cc->bounds.x),
									   static_cast<int>(tfc->tf.y + cc->bounds.y + cc->bounds.h / 2 + vc->velocity.y * delta),
									   cc->bounds.w,
									   cc->bounds.h / 2 + 8};

				if (SDL_HasIntersection(&dstRect, &cb))
					yec = true;
			}

			if (xec || yec)
			{
				if (cc->entityCollided == -1)
					cc->entityCollided = oe->id;

				if (pjc)
					pjc->stop = true;

				if (xec && yec && !opjc)
				{
					if (vc->velocity.x < 0.0f)
						tfc->tf.x += 0.5f;
					else if (vc->velocity.x > 0.0f)
						tfc->tf.x -= 0.5f;

					if (vc->velocity.y < 0.0f)
						tfc->tf.y += 0.5f;
					else if (vc->velocity.y > 0.0f)
						tfc->tf.y -= 0.5f;
				}
			}
			else
				cc->entityCollided = -1;
		}

		std::function<bool(SDL_FPoint)> hasCollision = [tfc, cc, vc, pjc, delta, &e](SDL_FPoint offset)
		{
			const SDL_Point pos{static_cast<int>(std::floor((tfc->tf.x + offset.x) / Tile::Properties::size)),
				static_cast<int>(std::floor((tfc->tf.y + offset.y) / Tile::Properties::size))};
	
			if (pos.x < 0 || pos.x >= cc->world.size.x || pos.y < 0 || pos.y >= cc->world.size.y)
				return true;
			
			auto &tile = cc->world.getTile(pos);

			if (tile->props.collidable)
			{
				if (tile->props.breakable)
				{
					if (pjc && !pjc->friendly)
						tile->props.health -= static_cast<int>(pjc->damage);
					else if (!pjc)
					{
						MeleeHostileComponent *mhc = e.getComponent<MeleeHostileComponent>();

						if (mhc)
						{
							mhc->attackTimer -= 1 * delta;

							if (mhc->attackTimer <= 0.0f)
							{
								tile->props.health -= static_cast<int>(mhc->damage);
								mhc->attackTimer = mhc->attackDelay;
							}
						}
					}
				}

				return true;
			}

			return false;
		};

		if (!xec)
		{
			if (vc->velocity.x < 0.0f)
			{
				if ((pjc && pjc->friendly) 
					|| std::floor((tfc->tf.x + cc->bounds.x + vc->velocity.x * delta) / Tile::Properties::size) >= 0.0f
					&& !hasCollision({cc->bounds.x + vc->velocity.x * delta, cc->bounds.y + cc->bounds.h / 3.0f})
					&& !hasCollision({cc->bounds.x + vc->velocity.x * delta, static_cast<float>(cc->bounds.y + cc->bounds.h)}))
				{
					tfc->tf.x += vc->velocity.x * delta;
				}
				else if (pjc)
					pjc->stop = true;
			}
			else if (vc->velocity.x > 0.0f)
			{
				if ((pjc && pjc->friendly) 
					|| std::floor((tfc->tf.x + cc->bounds.x + cc->bounds.w + vc->velocity.x * delta) / Tile::Properties::size) < cc->world.size.x
					&& !hasCollision({cc->bounds.x + cc->bounds.w + vc->velocity.x * delta, cc->bounds.y + cc->bounds.h / 3.0f})
					&& !hasCollision({cc->bounds.x + cc->bounds.w + vc->velocity.x * delta, static_cast<float>(cc->bounds.y + cc->bounds.h)}))
				{
					tfc->tf.x += vc->velocity.x * delta;
				}
				else if (pjc)
					pjc->stop = true;
			}
		}

		if (!yec)
		{
			if (vc->velocity.y < 0.0f)
			{
				if ((pjc && pjc->friendly) 
					|| std::floor((tfc->tf.y + cc->bounds.y + vc->velocity.y * delta) / Tile::Properties::size) >= 0.0f
					&& !hasCollision({static_cast<float>(cc->bounds.x), cc->bounds.y + cc->bounds.h / 3 + vc->velocity.y * delta})
					&& !hasCollision({cc->bounds.x + cc->bounds.w / 2.0f, cc->bounds.y + cc->bounds.h / 3 + vc->velocity.y * delta})
					&& !hasCollision({static_cast<float>(cc->bounds.x + cc->bounds.w), cc->bounds.y + cc->bounds.h / 3 + vc->velocity.y * delta}))
				{
					tfc->tf.y += vc->velocity.y * delta;
				}
				else if (pjc)
					pjc->stop = true;

			}
			else if (vc->velocity.y > 0.0f)
			{
				if ((pjc && pjc->friendly) 
					|| std::floor((tfc->tf.y + cc->bounds.y + cc->bounds.h + vc->velocity.y * delta) / Tile::Properties::size) < cc->world.size.y
					&& !hasCollision({static_cast<float>(cc->bounds.x), cc->bounds.y + cc->bounds.h + vc->velocity.y * delta})
					&& !hasCollision({cc->bounds.x + cc->bounds.w / 2.0f, cc->bounds.y + cc->bounds.h + vc->velocity.y * delta})
					&& !hasCollision({static_cast<float>(cc->bounds.x + cc->bounds.w), cc->bounds.y + cc->bounds.h + vc->velocity.y * delta}))
				{
					tfc->tf.y += vc->velocity.y * delta;
				}
				else if (pjc)
					pjc->stop = true;
			}
		}
	}
	else
	{
		tfc->tf.x += vc->velocity.x * delta;
		tfc->tf.y += vc->velocity.y * delta;
	}
}