#pragma once

class Entity;

class AttackSystem final
{
public:
	void update(Entity &e, float delta) noexcept;
};