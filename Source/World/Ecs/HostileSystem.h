#pragma once

class Entity;
class EntityManager;

class HostileSystem final
{
public:
	explicit HostileSystem(EntityManager &em) noexcept;

	void update(Entity &e, float delta) noexcept;

private:
	EntityManager &em;
};