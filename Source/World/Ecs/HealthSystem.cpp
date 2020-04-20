#include "HealthSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Component.h"
#include "../../Game.h"
#include "../../GameStates/GameOverMenuState.h"

HealthSystem::HealthSystem(EntityManager &em) noexcept
	: em{em}
{
}

void HealthSystem::update() noexcept
{
	for (auto it{em.entities.begin()}; it != em.entities.end(); ++it)
	{
		const HealthComponent *const hc = it->second.getComponent<HealthComponent>();
		const ProjectileComponent *const pjc = it->second.getComponent<ProjectileComponent>();

		if ((hc && hc->health <= 0) || (pjc && (pjc->alpha <= 0.0f || pjc->hit || pjc->stop)))
		{
			Entity &e = em.entities.at(it->first);

			const RenderComponent *const rc = e.getComponent<RenderComponent>();
			const CollisionComponent *const cc = e.getComponent<CollisionComponent>();

			if (rc && rc->compartmentalized)
			{
				for (auto oit{em.renderable.begin()}; oit != em.renderable.end(); ++oit)
				{
					if ((*oit)->id == it->first)
					{
						em.renderable.erase(oit);
						break;
					}
				}
			}

			if ((cc && cc->compartmentalized))
			{
				for (auto oit{em.collidables.begin()}; oit != em.collidables.end(); ++oit)
				{
					if ((*oit)->id == it->first)
					{
						em.collidables.erase(oit);
						break;
					}
				}
			}

			PlayerComponent *const pc = e.getComponent<PlayerComponent>();
			if (pc)
			{
				pc->game.gsm.add(std::make_unique<GameOverMenuState>(pc->game, pc->score));
				return;
			}

			it = em.entities.erase(it);
			if (it == em.entities.end())
				return;
		}
	}
}
