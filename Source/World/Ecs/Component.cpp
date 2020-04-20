#include "Component.h"

TransformComponent::TransformComponent(SDL_FRect tf) noexcept
	: tf{tf}
{
}

TextureComponent::TextureComponent(SDL_Texture *texture, SDL_Rect srcRect, const World &world) noexcept
	: texture{texture},
	srcRect{srcRect},
	world{world}
{
}

VelocityComponent::VelocityComponent(SDL_FPoint velocity, float accel, float maxSpeed, float slowdown) noexcept
	: velocity{velocity},
	accel{accel},
	maxSpeed{maxSpeed},
	slowdown{slowdown}
{
}

CollisionComponent::CollisionComponent(SDL_Rect bounds, World &world) noexcept
	: bounds{bounds},
	world{world}
{
}

CameraFocusComponent::CameraFocusComponent(World &world, const Game &game) noexcept
	: world{world},
	game{game}
{
}

HealthComponent::HealthComponent(float health) noexcept
	: health{health},
	maxHealth{health}
{
}

DirectionalAnimationComponent::DirectionalAnimationComponent(SDL_Texture *ss,
	SDL_Rect idleUpSrcRect, 
	SDL_Rect idleDownSrcRect, 
	SDL_Rect idleLeftSrcRect, 
	SDL_Rect idleRightSrcRect,
	Animation up,
	Animation down, 
	Animation left, 
	Animation right,
	Direction dir,
	const World &world) noexcept
	: ss{ss},
	idleUpSrcRect{idleUpSrcRect},
	idleDownSrcRect{idleDownSrcRect},
	idleLeftSrcRect{idleLeftSrcRect},
	idleRightSrcRect{idleRightSrcRect},
	up{up},
	down{down},
	left{left},
	right{right},
	dir{dir},
	world{world}
{
}

FollowComponent::FollowComponent(uint32_t target, float stopDistance) noexcept
	: target{target},
	stopDistance{stopDistance}
{
}

ProjectileComponent::ProjectileComponent(bool friendly, float damage, EntityManager &em, const Game &game) noexcept
	: friendly{friendly},
	damage{damage},
	em{em},
	game{game}
{
}

SingleAxisAnimationComponent::SingleAxisAnimationComponent(SDL_Texture *ss, SDL_Rect idleFirstSrcRect, SDL_Rect idleSecondSrcRect, Animation first, Animation second, Direction dir, const World &world) noexcept
	: ss{ss},
	idleFirstSrcRect{idleFirstSrcRect},
	idleSecondSrcRect{idleSecondSrcRect},
	first{first},
	second{second},
	dir{dir},
	world{world}
{
}

MeleeHostileComponent::MeleeHostileComponent(float damage, float attackDelay, std::uint32_t target) noexcept
	: damage{damage},
	attackDelay{attackDelay},
	target{target}
{
}

AngledTextureComponent::AngledTextureComponent(SDL_Texture *texture, SDL_Rect srcRect, double angle, const World &world) noexcept
	: texture{texture},
	srcRect{srcRect},
	angle{angle},
	world{world}
{
}

PlayerComponent::PlayerComponent(EntityManager &em, World &world, Game &game) noexcept
	: em{em},
	world{world},
	game{game}
{
}

RangedHostileComponent::RangedHostileComponent(float attackDelay, std::uint32_t target, Projectile projectile, EntityManager &em, const Game &game) noexcept
	: attackDelay{attackDelay},
	target{target},
	projectile{projectile},
	em{em},
	game{game}
{
}
