#pragma once

#include "Entity.h"
#include "RenderSystem.h"
#include "MovementSystem.h"
#include "CameraSystem.h"
#include "HealthSystem.h"
#include "HostileSystem.h"
#include "AttackSystem.h"
#include "Projectile.h"

#include <SDL_render.h>
#include <SDL_rect.h>

#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cstdint>

class EntityManager final
{
public:
	std::unordered_map<std::uint32_t, Entity> entities;
	std::vector<Entity *> renderable;
	std::vector<Entity *> collidables;

	std::uint32_t createEntity() noexcept;

	Entity *getEntity(std::uint32_t id) noexcept;
	const Entity *getEntity(std::uint32_t id) const noexcept;

	void update(float delta) noexcept;
	void render(SDL_Renderer *rdr) const noexcept;

	[[nodiscard]] int getHostileCount() const noexcept { return hostileCount; };

private:
	std::uint32_t nextId = 0;
	int hostileCount = 0;

	RenderSystem rdrsys{*this};
	const MovementSystem msys{*this};
	const CameraSystem camsys;
	HealthSystem healthSys{*this};
	HostileSystem hostileSys{*this};
	AttackSystem attackSys;
};

std::uint32_t createPlayer(EntityManager &em, SDL_FPoint pos,  DirectionalAnimationComponent::Direction dir, World &world, Game &game) noexcept;
std::uint32_t createSlime(EntityManager &em, SDL_FPoint pos,  SingleAxisAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createArcher(EntityManager &em, SDL_FPoint pos,  DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createDarkLord(EntityManager &em, SDL_FPoint pos,  DirectionalAnimationComponent::Direction dir, World &world, const Game &game, Projectile projectile) noexcept;
std::uint32_t createGoblin(EntityManager &em, SDL_FPoint pos,  DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createProjectile(EntityManager &em, Projectile type, SDL_FPoint pos, SDL_FPoint velocity, float maxSpeed, bool friendly, World &world, const Game &game) noexcept;
std::uint32_t createChest(EntityManager &em, SDL_FPoint pos, World &world, const Game &game) noexcept;
std::uint32_t createIceDemon(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createZombie(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createKnight(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;
std::uint32_t createLootLord(EntityManager &em, SDL_FPoint pos, DirectionalAnimationComponent::Direction dir, World &world, const Game &game) noexcept;