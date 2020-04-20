#pragma once

class EntityManager;
class Entity;

class MovementSystem final
{
public:
	enum class MovementType
	{
		none, dir1, dir2
	};

	explicit MovementSystem(EntityManager &em);

	void update(Entity &e, float delta) const noexcept;

private:
	EntityManager &em;
};