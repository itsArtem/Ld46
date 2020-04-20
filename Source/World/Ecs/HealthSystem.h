#pragma once

class EntityManager;
class Entity;

class HealthSystem final
{
public:
	explicit HealthSystem(EntityManager &em) noexcept;

	void update() noexcept;

private:
	EntityManager &em;
};