#include "EntityManager.h"
#include "Component.h"
#include "../../Game.h"
#include "../World.h"
#include "../../Random.h"

#include <memory>
#include <cmath>

std::uint32_t EntityManager::createEntity() noexcept
{
	entities.emplace(nextId, Entity{nextId});
	return nextId++;
}

Entity *EntityManager::getEntity(std::uint32_t id) noexcept
{
	if (entities.find(id) != entities.end())
		return &entities.at(id);

	return nullptr;
}

const Entity *EntityManager::getEntity(std::uint32_t id) const noexcept
{
	if (entities.find(id) != entities.end())
		return &entities.at(id);

	return nullptr;
}

void EntityManager::update(float delta) noexcept
{
	hostileCount = 0;

	for (auto &e : entities)
	{
		RenderComponent *rc = e.second.getComponent<RenderComponent>();
		CollisionComponent *cc = e.second.getComponent<CollisionComponent>();

		if (rc && !rc->compartmentalized)
		{
			renderable.emplace_back(&e.second);
			rc->compartmentalized = true;
		}

		if (cc && !cc->compartmentalized)
		{
			collidables.emplace_back(&e.second);
			cc->compartmentalized = true;
		}

		msys.update(e.second, delta);
		rdrsys.update(e.second, delta);
		camsys.update(e.second);
		hostileSys.update(e.second, delta);
		attackSys.update(e.second, delta);

		if (e.second.getComponent<MeleeHostileComponent>() || e.second.getComponent<RangedHostileComponent>())
			++hostileCount;
	}

	healthSys.update();
}

void EntityManager::render(SDL_Renderer *rdr) const noexcept
{
	rdrsys.render(rdr);
}

std::uint32_t createPlayer(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, Game &game) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);

	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 80.0f, 80.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(SDL_FPoint{0.0f, 0.0f}, 5000.0f, 200.0f, 5000.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{20, 0, 30, 80}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<DirectionalAnimationComponent>(ss,
		SDL_Rect{0, 0, 16, 16},
		SDL_Rect{0, 16, 16, 16},
		SDL_Rect{0, 32, 16, 16},
		SDL_Rect{0, 48, 16, 16},
		Animation{ss, {16, 0, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 16, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 32, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 48, 16, 16}, 4, 100.0f},
		dir,
		world));
	e->addComponent(std::make_unique<PlayerComponent>(em, world, game));
	e->addComponent(std::make_unique<CameraFocusComponent>(world, game));
	e->addComponent(std::make_unique<HealthComponent>(250.0f));

	return id;
}

std::uint32_t createSlime(EntityManager &em, SDL_FPoint pos, SingleAxisAnimationComponent::Direction dir, World &world, const Game &game) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);
	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 64.0f, 64.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(SDL_FPoint{0.0f, 0.0f}, 4000.0f, 100.0f, 4000.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{8, 8, 44, 58}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<SingleAxisAnimationComponent>(ss,
		SDL_Rect{80, 0, 16, 16},
		SDL_Rect{80, 16, 16, 16},
		Animation{ss, {96, 0, 16, 16}, 3, 150.0f},
		Animation{ss, {96, 16, 16, 16}, 3, 150.0f},
		dir,
		world));
	const std::uint32_t target = (getRandomBool() == false ? world.getPlayer() : world.getChest());
	e->addComponent(std::make_unique<FollowComponent>(target, 0.0f));
	e->addComponent(std::make_unique<MeleeHostileComponent>(5.0f, 1.0f, target, game));
	e->addComponent(std::make_unique<HealthComponent>(55.0f));

	return id;
}

std::uint32_t createArcher(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game, Projectile projectile) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);

	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 80.0f, 80.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(SDL_FPoint{0.0f, 0.0f}, 3000.0f, 100.0f, 3000.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{20, 0, 30, 80}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<DirectionalAnimationComponent>(ss,
		SDL_Rect{80, 32, 16, 16},
		SDL_Rect{80, 48, 16, 16},
		SDL_Rect{80, 64, 16, 16},
		SDL_Rect{80, 80, 16, 16},
		Animation{ss, {96, 32, 16, 16}, 4, 100.0f},
		Animation{ss, {96, 48, 16, 16}, 4, 100.0f},
		Animation{ss, {96, 64, 16, 16}, 4, 100.0f},
		Animation{ss, {96, 80, 16, 16}, 4, 100.0f},
		dir,
		world));
	e->addComponent(std::make_unique<HealthComponent>(60.0f));
	const std::uint32_t target = (getRandomBool() == false ? world.getPlayer() : world.getChest());
	e->addComponent(std::make_unique<FollowComponent>(target, 100.0f));
	e->addComponent(std::make_unique<RangedHostileComponent>(1.0f, target, projectile, em, game));

	return id;
}

std::uint32_t createDarkLord(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game, Projectile projectile) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);

	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 80.0f, 80.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(SDL_FPoint{0.0f, 0.0f}, 3000.0f, 100.0f, 3000.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{20, 0, 30, 80}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<DirectionalAnimationComponent>(ss,
		SDL_Rect{0, 128, 16, 16},
		SDL_Rect{0, 128 + 16, 16, 16},
		SDL_Rect{0, 128 + 32, 16, 16},
		SDL_Rect{0, 128 + 48, 16, 16},
		Animation{ss, {16, 128, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 128 + 16, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 128 + 32, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 128 + 48, 16, 16}, 4, 100.0f},
		dir,
		world));
	e->addComponent(std::make_unique<HealthComponent>(70.0f));
	const std::uint32_t target = (getRandomBool() == false ? world.getPlayer() : world.getChest());
	e->addComponent(std::make_unique<FollowComponent>(target, 150.0f));
	e->addComponent(std::make_unique<RangedHostileComponent>(1.3f, target, projectile, em, game));

	return id;
}

std::uint32_t createGoblin(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);

	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 80.0f, 80.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(SDL_FPoint{0.0f, 0.0f}, 3000.0f, 100.0f, 3000.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{20, 0, 30, 80}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<DirectionalAnimationComponent>(ss,
		SDL_Rect{0, 64, 16, 16},
		SDL_Rect{0, 64 + 16, 16, 16},
		SDL_Rect{0, 64 + 32, 16, 16},
		SDL_Rect{0, 64 + 48, 16, 16},
		Animation{ss, {16, 64, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 64 + 16, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 64 + 32, 16, 16}, 4, 100.0f},
		Animation{ss, {16, 64 + 48, 16, 16}, 4, 100.0f},
		dir,
		world));
	e->addComponent(std::make_unique<HealthComponent>(65.0f));
	const std::uint32_t target = (getRandomBool() == false ? world.getPlayer() : world.getChest());
	e->addComponent(std::make_unique<FollowComponent>(target, 0.0f));
	e->addComponent(std::make_unique<MeleeHostileComponent>(5.0f, 1.0f, target, game));

	return id;
}

std::uint32_t createProjectile(EntityManager &em, Projectile type, SDL_FPoint pos, SDL_FPoint velocity, float speed, bool friendly, World &world, const Game &game) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 24.0f, 24.0f}));
	e->addComponent(std::make_unique<VelocityComponent>(velocity, 0.0f, speed, 0.0f));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{0, 0, 24, 24}, world));
	e->addComponent(std::make_unique<RenderComponent>());

	double angle = std::atan2(static_cast<double>(velocity.y), static_cast<double>(velocity.x)) * 180 / M_PI + 90;

	switch (type)
	{
	case Projectile::arrow:
		e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 24.0f, 24.0f}));
		e->addComponent(std::make_unique<AngledTextureComponent>(game.texc.get(1), SDL_Rect{144, 0, 8, 8}, angle, world));
		e->addComponent(std::make_unique<ProjectileComponent>(friendly, 15.0f, em, game));
		break;


	case Projectile::dagger:
		e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 24.0f, 24.0f}));
		e->addComponent(std::make_unique<AngledTextureComponent>(game.texc.get(1), SDL_Rect{144, 8, 8, 8}, angle, world));
		e->addComponent(std::make_unique<ProjectileComponent>(friendly, 5.0f, em, game));
		break;


	case Projectile::spear:
		e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 24.0f, 48.0f}));
		e->addComponent(std::make_unique<AngledTextureComponent>(game.texc.get(1), SDL_Rect{152, 0, 8, 16}, angle, world));
		e->addComponent(std::make_unique<ProjectileComponent>(friendly, 20.0f, em, game));
		break;
	}

	return id;
}

std::uint32_t createChest(EntityManager &em, SDL_FPoint pos, World &world, const Game &game) noexcept
{
	const std::uint32_t id = em.createEntity();
	Entity *const e = em.getEntity(id);
	SDL_Texture *const ss = game.texc.get(1);

	e->addComponent(std::make_unique<TransformComponent>(SDL_FRect{pos.x, pos.y, 56.0f, 56.0f}));
	e->addComponent(std::make_unique<CollisionComponent>(SDL_Rect{3, 4, 49, 32}, world));
	e->addComponent(std::make_unique<TextureComponent>(game.texc.get(1), SDL_Rect{144, 16, 16, 16}, world));
	e->addComponent(std::make_unique<RenderComponent>());
	e->addComponent(std::make_unique<HealthComponent>(500.0f));

	return id;
}
