#pragma once

#include "MovementSystem.h"
#include "../../Graphics/Animation.h"
#include "Projectile.h"

#include <SDL_render.h>
#include <SDL_rect.h>

#include <cstdint>

class World;
class Game;
class EntityManager;

struct Component
{
public:
	Component(const Component &component) noexcept = default;
	Component(Component &&component) noexcept = default;

	virtual ~Component() = default;

	Component &operator =(const Component &component) noexcept = default;
	Component &operator =(Component && component) noexcept = default;

protected:
	Component() noexcept = default;
};

struct TransformComponent final : public Component
{
	SDL_FRect tf;

	TransformComponent(SDL_FRect tf) noexcept;
};

struct RenderComponent final : public Component
{
	bool compartmentalized = false;
};

struct TextureComponent final : public Component
{
	SDL_Texture *const texture;
	const SDL_Rect srcRect;

	const World &world;

	TextureComponent(SDL_Texture *texture, SDL_Rect srcRect, const World &world) noexcept;
};

struct AngledTextureComponent final : public Component
{
	SDL_Texture *const texture;
	const SDL_Rect srcRect;
	double angle;

	const World &world;
	
	AngledTextureComponent(SDL_Texture *texture, SDL_Rect srcRect, double angle, const World &world) noexcept;
};

struct DirectionalAnimationComponent final : public Component
{
	enum class Direction
	{
		up, down, left, right
	};

	SDL_Texture *const ss;
	const SDL_Rect idleUpSrcRect, idleDownSrcRect, idleLeftSrcRect, idleRightSrcRect;

	Animation up, down, left, right;
	Direction dir;

	const World &world;

	DirectionalAnimationComponent(SDL_Texture *ss, 
		SDL_Rect idleUpSrcRect,
		SDL_Rect idleDownSrcRect,
		SDL_Rect idleLeftSrcRect,
		SDL_Rect idleRightSrcRect,
		Animation up, 
		Animation down, 
		Animation left, 
		Animation right,
		Direction dir,
		const World &world) noexcept;
};

struct SingleAxisAnimationComponent final : public Component
{
	enum class Direction
	{
		neg, pos
	};

	SDL_Texture *const ss;
	const SDL_Rect idleFirstSrcRect, idleSecondSrcRect;

	Animation first, second;
	Direction dir;

	const World &world;

	SingleAxisAnimationComponent(SDL_Texture *ss,
		SDL_Rect idleFirstSrcRect,
		SDL_Rect idleSecondSrcRect,
		Animation first,
		Animation second,
		Direction dir,
		const World &world) noexcept;
};

struct VelocityComponent final : public Component
{
	SDL_FPoint velocity;

	const float accel;
	const float maxSpeed;

	const float slowdown;

	VelocityComponent(SDL_FPoint velocity, float accel, float maxSpeed, float slowdown) noexcept;
};

struct PlayerComponent final : public Component
{
	EntityManager &em;
	World &world;
	Game &game;

	float attackDelay = 400.0f;
	float attackTimer = attackDelay;
	
	int points = 400;
	int score = 0;
	bool preparation = true;
	const float prepareTime = 60.0f;
	float prepareTimer = prepareTime;
	int tileSelected = 3;

	explicit PlayerComponent(EntityManager &em, World &world, Game &game) noexcept;
};

struct CollisionComponent final : public Component
{
	const SDL_Rect bounds;
	World &world;

	bool compartmentalized = false;
	int entityCollided = -1;

	CollisionComponent(SDL_Rect bounds, World &world) noexcept;
};

struct CameraFocusComponent final : public Component
{
	World &world;
	const Game &game;

	CameraFocusComponent(World &world, const Game &game) noexcept;
};

struct RandomMovementComponent final : public Component
{
	MovementSystem::MovementType x = MovementSystem::MovementType::none;
	MovementSystem::MovementType y = MovementSystem::MovementType::none;
};

struct FollowComponent final : public Component
{
	const uint32_t target;
	float stopDistance;

	FollowComponent(uint32_t target, float stopDistance) noexcept;
};

struct HealthComponent final : public Component
{
	float health;
	const float maxHealth;

	HealthComponent(float health) noexcept;
};

struct ProjectileComponent final : public Component
{
	const bool friendly;
	float damage;

	float timer = 20.0f;
	float alpha = 255.0f;

	bool stop = false;
	EntityManager &em;
	bool hit = false;

	const Game &game;

	ProjectileComponent(bool friendly, float damage, EntityManager &em, const Game &game) noexcept;
};

struct MeleeHostileComponent final : public Component
{
	const float damage;
	std::uint32_t target;

	const float attackDelay;
	float attackTimer = 0.0f;
	const Game &game;

	MeleeHostileComponent(float damage, float attackDelay, std::uint32_t target, const Game &game) noexcept;
};

struct RangedHostileComponent final : public Component
{
	std::uint32_t target;

	const float attackDelay;
	float attackTimer = 0.0f;
	Projectile projectile;
	EntityManager &em;
	const Game &game;

	RangedHostileComponent(float attackDelay, std::uint32_t target, Projectile projectile, EntityManager &em, const Game &game) noexcept;
};